package com.github.cqu_bdsc.collision_warning_system;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.baidu.location.BDAbstractLocationListener;
import com.baidu.location.BDLocation;
import com.baidu.location.LocationClient;
import com.baidu.location.LocationClientOption;
import com.github.cqu_bdsc.collision_warning_system.DAO.Message;
import com.github.cqu_bdsc.collision_warning_system.infor.InfoThread;
import com.github.cqu_bdsc.collision_warning_system.udp.ReceiveThread;
import com.github.cqu_bdsc.collision_warning_system.DAO.Result;
import com.github.cqu_bdsc.collision_warning_system.udp.SendService;

import java.io.IOException;
import java.util.Objects;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class MainActivity extends AppCompatActivity {

    private InfoThread infoThread;

    public LocationClient mLocationClient = null;
    private MyLocationListener myListener = new MyLocationListener();
    //BDAbstractLocationListener为7.2版本新增的Abstract类型的监听接口
    //原有BDLocationListener接口暂时同步保留。具体介绍请参考后文中的说明

    private static final String SERVER_IP = "192.168.1.80";
    private static final String SERVER_PORT = "4040";

    private String Server_Add = "192.168.1.80";

    private BroadcastReceiver broadcastReceiver = null;
    private final IntentFilter intentFilter = new IntentFilter();
    private Button      btnPing;//以下是可视化程序里面的常量
    private Button        btnSend;

    private EditText    etIp;
    private TextView    tvPingResult;
    private TextView    tv_showid;
    private TextView    tv_warning;
    private TextView    tv_time;
    private TextView    tv_distance;
    private TextView    tv_log;

    private EditText    et_id;
    private EditText    et_timeStamp;
    private EditText    et_speed;
    private EditText    et_direction;
    private EditText    et_lat;
    private EditText    et_lon;
    private EditText    et_ace;


    @Override
    protected void onResume() {
        super.onResume();
        broadcastReceiver = new RecationBroadcast();
        registerReceiver(broadcastReceiver, intentFilter);//广播注册
    }


    @Override
    protected void onPause() {
        super.onPause();
        unregisterReceiver(broadcastReceiver);
    }


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);//加载布局

        intentFilter.addAction(ReceiveThread.ACTION_STRING);
        intentFilter.addAction(ReceiveThread.ACTION_JSON);
        intentFilter.addAction(InfoThread.ACTION_INFORMATION);


        etIp        = (EditText) findViewById(R.id.et_ipAdd);
        btnPing     = (Button)   findViewById(R.id.btn_testPing);
        tvPingResult= (TextView) findViewById(R.id.tvPingResult);

        tvPingResult= (TextView) findViewById(R.id.tvPingResult);


        et_id        = (EditText) findViewById(R.id.et_id);
        et_timeStamp        = (EditText) findViewById(R.id.et_timeStamp);
        et_speed        = (EditText) findViewById(R.id.et_speed);
        et_direction        = (EditText) findViewById(R.id.et_direction);
        et_lat        = (EditText) findViewById(R.id.et_lat);
        et_lon        = (EditText) findViewById(R.id.et_lon);
        et_ace        = (EditText) findViewById(R.id.et_ace);

        tv_showid = (TextView) findViewById(R.id.tv_showid);
        tv_distance = (TextView)  findViewById(R.id.tv_distance);
        tv_time    = (TextView) findViewById(R.id.tv_time);
        tv_warning = (TextView) findViewById(R.id.tv_warning);

        tv_log = (TextView) findViewById(R.id.textLog);

        btnSend     = (Button)   findViewById(R.id.btn_send);


        btnPing.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (etIp.getEditableText().toString().equals("")){    //使用默认IP地址
                    if (isConnectedServer(SERVER_IP)){
                        tvPingResult.setText("Ping "+SERVER_IP+ "Success.");
                    } else {
                        tvPingResult.setText("Ping "+SERVER_IP+ "Failed.");
                    }
                } else {
                    String ipAdd = (String) etIp.getEditableText().toString();
                    if (isIp(ipAdd)){  //ip匹配
                        setServer_Add(ipAdd);
                        if (isConnectedServer(ipAdd)){
                            tvPingResult.setText("Ping"+ipAdd+ "Success.");
                        } else {
                            tvPingResult.setText("Ping"+ipAdd+ "Failed.");
                        }
                    } else { //ip不匹配
                        tvPingResult.setText("请重新输入IP.");
                    }

                }

            }
        });

        btnSend.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {//只要一点发送，就有数据的录入，获取，转换，以及数据json格式的转化过程

                String id = et_id.getEditableText().toString();//将输入的格式统一化为string型
                String timeStamp = et_timeStamp.getEditableText().toString();
                String speed = et_speed.getEditableText().toString();
                String direction = et_direction.getEditableText().toString();
                String lat = et_lat.getEditableText().toString();
                String lon = et_lon.getEditableText().toString();
                String ace = et_ace.getEditableText().toString();

                Message message = new Message();
                message.setId(Integer.valueOf(id));//将上面得到的字符串类型的数据转化为具体相对应的类型
                message.setTimeStamp(Integer.valueOf(timeStamp));
                message.setSpeed(Double.valueOf(speed));
                message.setDirection(Integer.valueOf(direction));
                message.setLat(Double.valueOf(lat));
                message.setLon(Double.valueOf(lon));
                message.setAce(Double.valueOf(ace));
//现在Message里面已经有对应格式的数据，接下来是将数据转化为json格式。

                Intent intent = new Intent(MainActivity.this, SendService.class);//跳转到SendService活动
                intent.setAction(SendService.ACTION_SEND_JSON);//将执行服务的活动，现在并不执行，只是告诉android，我们要调用哪个功能。
                intent.putExtra(SendService.EXTRAS_HOST,getServer_Add());//将执行服务活动的限制，IP地址，端口号，还有
                intent.putExtra(SendService.EXTRAS_PORT,SERVER_PORT);
                intent.putExtra(SendService.EXTRAS_JSON,message);
                startService(intent);//现在真正执行服务

            }
        });


        /**
         * 运行接收线程
         */
        ReceiveThread receiveThread = new ReceiveThread(MainActivity.this, getServer_Add());
        receiveThread.start();

        /**
         * 百度定位
         */
        mLocationClient = new LocationClient(getApplicationContext());
        //声明LocationClient类
        mLocationClient.registerLocationListener(myListener);
        //注册监听函数
        LocationClientOption option = new LocationClientOption();

        option.setLocationMode(LocationClientOption.LocationMode.Hight_Accuracy);
        //可选，设置定位模式，默认高精度
        //LocationMode.Hight_Accuracy：高精度；
        //LocationMode. Battery_Saving：低功耗；
        //LocationMode. Device_Sensors：仅使用设备；

        option.setCoorType("bd09ll");
        //可选，设置返回经纬度坐标类型，默认gcj02
        //gcj02：国测局坐标；
        //bd09ll：百度经纬度坐标；
        //bd09：百度墨卡托坐标；
        //海外地区定位，无需设置坐标类型，统一返回wgs84类型坐标

        option.setScanSpan(1000);
        //可选，设置发起定位请求的间隔，int类型，单位ms
        //如果设置为0，则代表单次定位，即仅定位一次，默认为0
        //如果设置非0，需设置1000ms以上才有效

        option.setOpenGps(true);
        //可选，设置是否使用gps，默认false
        //使用高精度和仅用设备两种定位模式的，参数必须设置为true

        option.setLocationNotify(true);
        //可选，设置是否当GPS有效时按照1S/1次频率输出GPS结果，默认false

        option.setIgnoreKillProcess(false);
        //可选，定位SDK内部是一个service，并放到了独立进程。
        //设置是否在stop的时候杀死这个进程，默认（建议）不杀死，即setIgnoreKillProcess(true)

        option.SetIgnoreCacheException(true);
        //可选，设置是否收集Crash信息，默认收集，即参数为false

        option.setWifiCacheTimeOut(5*60*1000);
        //可选，7.2版本新增能力
        //如果设置了该接口，首次启动定位时，会先判断当前WiFi是否超出有效期，若超出有效期，会先重新扫描WiFi，然后定位

        option.setEnableSimulateGps(false);
        //可选，设置是否需要过滤GPS仿真结果，默认需要，即参数为false

        mLocationClient.setLocOption(option);
        //mLocationClient为第二步初始化过的LocationClient对象
        //需将配置好的LocationClientOption对象，通过setLocOption方法传递给LocationClient对象使用
        //更多LocationClientOption的配置，请参照类参考中LocationClientOption类的详细说明

        mLocationClient.start();
        //mLocationClient为第二步初始化过的LocationClient对象
        //调用LocationClient的start()方法，便可发起定位请求

        infoThread = new InfoThread(MainActivity.this);
        infoThread.start();

    }

    public boolean isConnectedServer(String ip) {

        Runtime runtime = Runtime.getRuntime();
        try
        {
            Process  mIpAddrProcess = runtime.exec("/system/bin/ping -c 1 "+ip);
            int mExitValue = mIpAddrProcess.waitFor();
            System.out.println(" mExitValue "+mExitValue);
            if(mExitValue==0){
                return true;
            }else{
                return false;
            }
        } catch (IOException | InterruptedException e){
            e.printStackTrace();
            return false;
        }
    }

    //ip地址是否匹配
    public boolean isIp(String ipAddress){
        String ip = "([1-9]|[1-9]\\d|1\\d{2}|2[0-4]\\d|25[0-5])(\\.(\\d|[1-9]\\d|1\\d{2}|2[0-4]\\d|25[0-5])){3}";
        Pattern pattern = Pattern.compile(ip);
        Matcher matcher = pattern.matcher(ipAddress);
        return matcher.matches();
    }

    public String getServer_Add() {
        return Server_Add;
    }

    public void setServer_Add(String server_Add) {
        Server_Add = server_Add;
    }


    /**
     * 获取百度GPS坐标
     */
    public class MyLocationListener extends BDAbstractLocationListener {
        @Override
        public void onReceiveLocation(BDLocation location){
            //此处的BDLocation为定位结果信息类，通过它的各种get方法可获取定位相关的全部结果
            //以下只列举部分获取经纬度相关（常用）的结果信息
            //更多结果信息获取说明，请参照类参考中BDLocation类中的说明

            double latitude = location.getLatitude();    //获取纬度信息
            double longitude = location.getLongitude();    //获取经度信息
            float radius = location.getRadius();    //获取定位精度，默认值为0.0f
            float speed = location.getSpeed();

            String coorType = location.getCoorType();
            //获取经纬度坐标类型，以LocationClientOption中设置过的坐标类型为准

            int errorCode = location.getLocType();
            //获取定位类型、定位错误返回码，具体信息可参照类参考中BDLocation类中的说明

            Message message = new Message();
            message.setLat(latitude);
            message.setLon(longitude);
            message.setSpeed(speed);

            Intent intent = new Intent();
            intent.setAction(InfoThread.ACTION_INFORMATION);//告诉android将要执行什么功能
            intent.putExtra(InfoThread.EXTRAR_INFORMATION, message);//信息
            getApplicationContext().sendBroadcast(intent);//广播信息

            sendLog(String.valueOf(errorCode));

        }
    }

    public void sendLog(String log){
        Intent intent = new Intent();
        intent.setAction(ReceiveThread.ACTION_STRING);//告诉android将要执行什么功能
        intent.putExtra(ReceiveThread.STRING_CONTEXT, log);//信息
        getApplicationContext().sendBroadcast(intent);//广播信息
    }

    public class RecationBroadcast extends BroadcastReceiver {

        @Override//复写onReceive方法
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            switch (action){
                case ReceiveThread.ACTION_STRING:
                    String message = intent.getExtras().getString(ReceiveThread.STRING_CONTEXT);
                    tv_log.setText(message);
                    break;
                case ReceiveThread.ACTION_JSON:
                    Result result = (Result) Objects.requireNonNull(intent.getExtras()).get(ReceiveThread.JSON_CONTEXT);
                    if (result != null){
                        tv_showid.setText(String.valueOf(result.getId()));
                        tv_warning.setText(String.valueOf(result.isWarning()));
                        tv_distance.setText(String.valueOf(result.getDistance()));
                        tv_time.setText(String.valueOf(result.getTime()));
                    } else {
                        tv_showid.setText("0");
                        tv_warning.setText("0");
                        tv_distance.setText("0");
                        tv_time.setText("0");
                    }
                    break;
                case InfoThread.ACTION_INFORMATION:
                    Message intentMessage = (Message) Objects.requireNonNull(intent.getExtras()).get(InfoThread.EXTRAR_INFORMATION);
                    if (intentMessage.getId() != Message.ERROR_VALUE){
                        et_id.setText(String.valueOf(intentMessage.getId()));
                    }
                    if (intentMessage.getAce() != Message.ERROR_VALUE){
                        et_ace.setText(String.valueOf(intentMessage.getAce()));
                    }
                    if (intentMessage.getDirection() != Message.ERROR_VALUE){
                        et_direction.setText(String.valueOf(intentMessage.getDirection()));
                    }
                    if (intentMessage.getLat() != Message.ERROR_VALUE){
                        et_lat.setText(String.valueOf(intentMessage.getLat()));
                    }
                    if (intentMessage.getLon() != Message.ERROR_VALUE){
                        et_lon.setText(String.valueOf(intentMessage.getLon()));
                    }
                    if (intentMessage.getSpeed() != Message.ERROR_VALUE){
                        et_speed.setText(String.valueOf(intentMessage.getSpeed()));
                    }
                    if (intentMessage.getTimeStamp() != Message.ERROR_VALUE){
                        et_timeStamp.setText(String.valueOf(intentMessage.getTimeStamp()));
                    }
                    break;
                default:
                    break;
            }
        }
    }

}

