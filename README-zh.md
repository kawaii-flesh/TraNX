# TraNX.ovl
TraNX - Nintendo Switch 屏幕翻译器

Инструкции на русском языке: [README.md](README.md)

English Introduction: [README-en.md](README-en.md)

## 安装与启动
1. 启动 [TraNX-Server](https://github.com/kawaii-flesh/TraNX-Server)
2. 安装 [具有扩展内存的 nx-ovlloader](https://github.com/kawaii-flesh/nx-ovlloader/releases)
3. 从可用版本中下载 `TraNX.ovl` 文件
4. 将 `TraNX.ovl` 文件放置在以下路径 -> `sd:/switch/.overlays`
5. 启动 `ovlmenu`
6. 从覆盖层列表中选择 `TraNX` (首次启动后将在 `sd:/config/TraNX/config.ini` 创建配置文件)
7. 退出覆盖层 (按下`左摇杆`，按下十字键的`左方向键`)
8. 在配置文件中，将 `uploadURL` 字段更改为运行 `TraNX-Server` 的`主机地址`
9. 重启 `TraNX`

## 控制
启动 `TraNX` 后，左上角会显示一个矩形，其颜色表示当前模式：

`绿色` - 输入焦点在游戏上

`黄色` - 交互模式：

从 v1.1.0 版本开始，可以在两种模式（绿色和黄色）下使用组合键。
为此，需要在配置文件 `sd:/config/Trunk/config.ini` 中的组合键前加上感叹号：
```ini
upload Combo=!RS ; 现在可以在两种模式下执行翻译请求，无需切换
```

选择翻译区域：
1. 按住十字键的`右方向键`
2. 通过触摸屏选择区域
3. 松开`右方向键`

选择翻译输出区域 (如果未选择，则输出到翻译区域)：
1. 按住十字键的`上方向键`
2. 通过触摸屏选择区域
3. 松开`上方向键`


- `右摇杆` - 发送翻译请求
- `下方向键` - 清除屏幕
- `左方向键` - 退出覆盖层
- `左摇杆` - 返回绿色模式

## 截图

![1](/screenshots/1.jpg)

![2](/screenshots/2.jpg)

![3](/screenshots/3.jpg)

![4](/screenshots/4.jpg)