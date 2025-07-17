#include "discover.hpp"

#include <arpa/inet.h>
#include <cstring>
#include <netinet/in.h>
#include <stdexcept>
#include <string>
#include <switch.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

namespace utils {

std::string discoverServer(uint16_t discoveryPort, int timeoutMs) {
  int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (sock < 0) {
    throw std::runtime_error("Failed to create socket");
  }

  int broadcastEnable = 1;
  if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcastEnable,
                 sizeof(broadcastEnable)) < 0) {
    close(sock);
    throw std::runtime_error("Failed to enable broadcast on socket");
  }

  timeval timeout;
  timeout.tv_sec = timeoutMs / 1000;
  timeout.tv_usec = (timeoutMs % 1000) * 1000;

  setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

  sockaddr_in broadcastAddr{};
  broadcastAddr.sin_family = AF_INET;
  broadcastAddr.sin_port = htons(discoveryPort);

  uint32_t ip, subnet_mask;
  if (nifmGetCurrentIpConfigInfo(&ip, &subnet_mask, nullptr, nullptr,
                                 nullptr) != 0) {
    close(sock);
    throw std::runtime_error("Failed to get IP config info");
  }

  broadcastAddr.sin_addr.s_addr = ip | ~subnet_mask;

  const char *message = "DISCOVER_TRANX_SERVER";
  if (sendto(sock, message, strlen(message), 0,
             reinterpret_cast<sockaddr *>(&broadcastAddr),
             sizeof(broadcastAddr)) < 0) {
    close(sock);
    throw std::runtime_error("Failed to send broadcast message");
  }

  char buffer[256];
  sockaddr_in senderAddr{};
  socklen_t senderAddrLen = sizeof(senderAddr);
  ssize_t recvLen =
      recvfrom(sock, buffer, sizeof(buffer) - 1, 0,
               reinterpret_cast<sockaddr *>(&senderAddr), &senderAddrLen);

  close(sock);

  if (recvLen < 0) {
    throw std::runtime_error("Timeout waiting for discovery response");
  }

  buffer[recvLen] = '\0';
  std::string received(buffer);

  if (received.rfind("TRANX_SERVER:", 0) != 0) {
    throw std::runtime_error("Invalid discovery response");
  }

  std::string portStr = received.substr(std::string("TRANX_SERVER:").size());

  const char *ipCStr = inet_ntoa(senderAddr.sin_addr);
  if (!ipCStr) {
    throw std::runtime_error("Failed to parse sender IP");
  }

  return std::string(ipCStr) + ":" + portStr;
}

} // namespace utils
