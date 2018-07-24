## Android 开发
### 负责人
[张浪](https://github.com/zhanglang111) 、[许新操](https://github.com/neardws) 
### Feature

* Application name : collision-warning-system
* Company domain: cqu-bdsc.github.com
* Package name: com.github.cqu_bdsc.collision_warning_system
* Taget Api : API 21: Android 5.0 Lollipop

### debug
**bug.1**
星期一, 16. 七月 2018 03:25下午 

	07-16 15:19:50.881 12121-15741/com.github.cqu_bdsc.collision_warning_system W/System.err: java.net.BindException: Address already in use
	07-16 15:19:50.882 12121-15741/com.github.cqu_bdsc.collision_warning_system W/System.err:     at java.net.PlainDatagramSocketImpl.bind0(Native Method)
	        at java.net.AbstractPlainDatagramSocketImpl.bind(AbstractPlainDatagramSocketImpl.java:96)
	        at java.net.DatagramSocket.bind(DatagramSocket.java:390)
	        at java.net.DatagramSocket.<init>(DatagramSocket.java:245)
	        at java.net.DatagramSocket.<init>(DatagramSocket.java:298)
	        at java.net.DatagramSocket.<init>(DatagramSocket.java:270)
	        at com.github.cqu_bdsc.collision_warning_system.udp.SendService.onHandleIntent(SendService.java:52)
	        at android.app.IntentService$ServiceHandler.handleMessage(IntentService.java:67)
	        at android.os.Handler.dispatchMessage(Handler.java:105)
	        at android.os.Looper.loop(Looper.java:156)
	        at android.os.HandlerThread.run(HandlerThread.java:61)

### 模块化开发
#### UDP 通信模块
接收功能已实现，发送功能有问题
