package com.github.cqu_bdsc.collision_warning_system;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.drm.DrmManagerClient;
import android.graphics.Color;
import android.media.Ringtone;
import android.media.RingtoneManager;
import android.net.Uri;
import android.os.Vibrator;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.method.ScrollingMovementMethod;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;

import com.baidu.location.BDAbstractLocationListener;
import com.baidu.location.BDLocation;
import com.baidu.location.LocationClient;
import com.baidu.location.LocationClientOption;
import com.github.cqu_bdsc.collision_warning_system.DAO.Message;
import com.github.cqu_bdsc.collision_warning_system.database.DBTool;
import com.github.cqu_bdsc.collision_warning_system.infor.InfoThread;
import com.github.cqu_bdsc.collision_warning_system.udp.ReceiveThread;
import com.github.cqu_bdsc.collision_warning_system.DAO.Result;
import com.github.cqu_bdsc.collision_warning_system.udp.SendService;
import com.raizlabs.android.dbflow.config.FlowManager;
import com.raizlabs.android.dbflow.list.IFlowCursorIterator;

import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Objects;
import java.util.regex.Matcher;
import java.util.regex.Pattern;


public class MainActivity extends AppCompatActivity {

    private InfoThread infoThread;
    private ReceiveThread receiveThread;
    private SendThread sendThread;
    /*************************************
     *  时间同步后，发送的时间戳：
     *  timeStamp = currentTimeStamp - baseStamp + serverStamp;
     *  offsetStamp =  currentTimeStamp - baseStamp
     *  详细信息请参见Result 类
     */
    private long baseStamp = 0;
    private long serverStamp = 0;


    public LocationClient mLocationClient = null;
    private MyLocationListener myListener = new MyLocationListener();
    //BDAbstractLocationListener为7.2版本新增的Abstract类型的监听接口
    //原有BDLocationListener接口暂时同步保留。具体介绍请参考后文中的说明

    public static final String ACTION_SEND_MESSAGE = "ACTION_SEND_MESSAGE";
    public static final String ACTION_SQL = "ACTION_SQL";
    private static final String SERVER_IP = "192.168.1.80";
    private static final String SERVER_PORT = "4040";

    private String Server_Add = "192.168.1.80";

    private BroadcastReceiver broadcastReceiver = null;
    private final IntentFilter intentFilter = new IntentFilter();
    private Button      btnPing;//以下是可视化程序里面的常量
    private Button      btnSend;
    private Button      btnStart;
    private Button      btnQuery;
    private Button      btnBackup;

    private EditText    etIp;
    private TextView    tvPingResult;
    private TextView    tv_showid;
    private TextView    tv_warning;
    private TextView    tv_time;
    private TextView    tv_distance;
    private TextView    tv_log;
    private TextView    tv_mac;
    private TextView    tv_gps;
    private TextView    tv_sql;

    private EditText    et_id;
    private EditText    et_timeStamp;
    private EditText    et_speed;
    private EditText    et_direction;
    private EditText    et_lat;
    private EditText    et_lon;
    private EditText    et_ace;

    private ImageView   img_warning;

    boolean sendThreadStart = false;

    boolean isSendThreadStart(){
        return sendThreadStart;
    }

    void setSendThreadStart(boolean isStart){
        this.sendThreadStart = isStart;
    }

    public void setBaseStamp(long baseStamp) {
        this.baseStamp = baseStamp;
    }

    public long getBaseStamp() {
        return baseStamp;
    }

    public void setServerStamp(long serverStamp) {
        this.serverStamp = serverStamp;
    }

    public long getServerStamp() {
        return serverStamp;
    }

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
        FlowManager.init(this);  //初始化DBFlow

        setContentView(R.layout.activity_main);//加载布局

        intentFilter.addAction(ReceiveThread.ACTION_STRING);
        intentFilter.addAction(ReceiveThread.ACTION_JSON);
        intentFilter.addAction(InfoThread.ACTION_INFORMATION);
        intentFilter.addAction(MainActivity.ACTION_SEND_MESSAGE);
        intentFilter.addAction(MainActivity.ACTION_SQL);


        etIp        = (EditText) findViewById(R.id.et_ipAdd);
        btnPing     = (Button)   findViewById(R.id.btn_testPing);
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
        tv_log.setMovementMethod(ScrollingMovementMethod.getInstance());
        tv_mac = (TextView) findViewById(R.id.tv_mac);
        tv_gps = (TextView) findViewById(R.id.tv_gps);
        tv_sql = (TextView) findViewById(R.id.tv_sql);


        img_warning = (ImageView) findViewById(R.id.img_warning);
        img_warning.setImageDrawable(getResources().getDrawable(R.mipmap.ic_safe));

        btnSend     = (Button)   findViewById(R.id.btn_send);
        btnStart    = (Button)   findViewById(R.id.btn_start);
        btnQuery    = (Button)   findViewById(R.id.btn_query);
        btnBackup   = (Button)   findViewById(R.id.btn_backup);

        btnPing.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (etIp.getEditableText().toString().equals("")){    //使用默认IP地址
                    if (isConnectedServer(SERVER_IP)){
                        tvPingResult.setText("Ping "+SERVER_IP+ "Success.");
                        addLogToDB("Ping "+SERVER_IP+ "Success.");
                    } else {
                        tvPingResult.setText("Ping "+SERVER_IP+ "Failed.");
                        addLogToDB("Ping "+SERVER_IP+ "Failed.");
                    }
                } else {
                    String ipAdd = (String) etIp.getEditableText().toString();
                    if (isIp(ipAdd)){  //ip匹配
                        setServer_Add(ipAdd);
                        if (isConnectedServer(ipAdd)){
                            tvPingResult.setText("Ping"+ipAdd+ "Success.");
                            addLogToDB("Ping"+ipAdd+ "Success.");
                        } else {
                            tvPingResult.setText("Ping"+ipAdd+ "Failed.");
                            addLogToDB("Ping"+ipAdd+ "Failed.");
                        }
                    } else { //ip不匹配
                        tvPingResult.setText("请重新输入IP.");
                        addLogToDB("请重新输入IP.");
                    }

                }

            }
        });

        btnBackup.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                addLogToDB("备份数据库");
                Intent intent = new Intent(MainActivity.this, DBTool.class);
                intent.setAction(DBTool.ACTION_COPY);
                startService(intent);
            }
        });

        /***********************************************
         * 现为时间同步按钮
         */
        btnSend.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {//只要一点发送，就有数据的录入，获取，转换，以及数据json格式的转化过程
              sendTimeStampMessage();
              addLogToDB("发起时间同步");
            }
        });


        /**
         * 注册接收线程、发送线程
         */
        receiveThread = new ReceiveThread(MainActivity.this, getServer_Add());
        infoThread = new InfoThread(MainActivity.this);
        sendThread = new SendThread();

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

        /*************
         * 开启应用时启动线程
         */
        mLocationClient.start();
        receiveThread.start();
        infoThread.start();



        btnStart.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (isSendThreadStart()){
                    sendThread.stopMe();
                    sendLog("停止线程......");
                    addLogToDB("停止线程...");
                    setSendThreadStart(false);
                    btnStart.setText("Start");
                } else {
                    sendThread.start();
                    sendLog("开始线程......");
                    addLogToDB("开始线程...");
                    setSendThreadStart(true);
                    btnStart.setText("Stop");
                }
            }
        });

        btnQuery.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this, DBTool.class);
                intent.setAction(DBTool.ACTION_SELETE);
                intent.putExtra(DBTool.TABLE_NAME, DBTool.TABLE_ALL);
                startService(intent);
            }
        });

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

            /********************************
             *    注意注释
             ***************************/
//            if (61 != errorCode){
//                message.setLat(29.5699345275);
//                message.setLon(106.4775258188);
//                message.setSpeed(1);
//            }

            Intent intent = new Intent();
            intent.setAction(InfoThread.ACTION_INFORMATION);//告诉android将要执行什么功能
            intent.putExtra(InfoThread.EXTRAR_INFORMATION, message);//信息
            getApplicationContext().sendBroadcast(intent);//广播信息

            tv_gps.setText(String.valueOf(errorCode));

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
                    tv_log.append(message);
                    tv_log.append("\n");
                    break;
                case MainActivity.ACTION_SQL:
                    String sqlmessage = intent.getExtras().getString(ReceiveThread.STRING_CONTEXT);
                    tv_sql.setText(sqlmessage);
                    break;
                case ReceiveThread.ACTION_JSON:
                    Result result = (Result) Objects.requireNonNull(intent.getExtras()).get(ReceiveThread.JSON_CONTEXT);
                    long nowTimwStamp = System.currentTimeMillis();
                    if (result != null){
                        int id = result.getId();
                        if (String.valueOf(id).equals(et_id.getEditableText().toString())){
                            String type = result.getType();
                            boolean warning;
                            double distance;
                            int time;
                            long sendTimeStamp;
                            long timeStamp;
                            long receiverTimeStamp;
                            switch (type){
                                case Result.TYPE_RESULT:
                                    warning = result.isWarning();
                                    if (warning){
                                        getWarning();
                                    } else {
                                        getSafe();
                                    }
                                    distance = result.getDistance();
                                    time = result.getTime();
                                    sendTimeStamp = result.getSendTimeStamp();
                                    tvPingResult.setText("TYPE_RESULT");
                                    tv_showid.setText(String.valueOf(id));
                                    tv_warning.setText(String.valueOf(warning));
                                    tv_distance.setText(String.valueOf(distance));
                                    tv_time.setText(String.valueOf(time));
                                    long delay = nowTimwStamp - sendTimeStamp;
                                    addResultToDB(id, time, distance, warning, nowTimwStamp, sendTimeStamp, delay);
                                    break;
                                case Result.TYPE_TIME_SYNC_RESULT:
                                    tvPingResult.setText("TYPE_TIME_SYNC_RESULT");
                                    timeStamp = result.getTimeStamp();
                                    sendTimeStamp =  result.getSendTimeStamp();
                                    receiverTimeStamp = result.getReceiverTimeStamp();
                                    delay = ((nowTimwStamp - timeStamp) - (sendTimeStamp - receiverTimeStamp)) / 2;
                                    long serverTimeStamp = sendTimeStamp + delay;
                                    setBaseStamp(nowTimwStamp);
                                    setServerStamp(serverTimeStamp);
                                    tvPingResult.setText("Time Sync success");
                                    break;
                                default:
                                    break;
                            }
                        } else {
                            tvPingResult.setText("Result is from else id:"+String.valueOf(result.getId()));
                        }
                    } else {
                        tvPingResult.setText("Result is null");
                    }
                    break;
                case InfoThread.ACTION_INFORMATION:
                    Message intentMessage = (Message) Objects.requireNonNull(intent.getExtras()).get(InfoThread.EXTRAR_INFORMATION);
//                    if (intentMessage.getId() != Message.ERROR_VALUE){
//                        et_id.setText(String.valueOf(intentMessage.getId()));
//                    }
//                    if (intentMessage.getAce() != Message.ERROR_VALUE){
//                        et_ace.setText(String.valueOf(intentMessage.getAce()));
//                    }
//                    if (intentMessage.getDirection() != Message.ERROR_VALUE){
//                        et_direction.setText(String.valueOf(intentMessage.getDirection()));
//                    }
//                    if (intentMessage.getLat() != Message.ERROR_VALUE){
//                        et_lat.setText(String.valueOf(intentMessage.getLat()));
//                    }
//                    if (intentMessage.getLon() != Message.ERROR_VALUE){
//                        et_lon.setText(String.valueOf(intentMessage.getLon()));
//                    }
//                    if (intentMessage.getSpeed() != Message.ERROR_VALUE){
//                        et_speed.setText(String.valueOf(intentMessage.getSpeed()));
//                    }
                    if (intentMessage.getTimeStamp() != Message.ERROR_VALUE){
                        if (0 != getBaseStamp() && 0 != getServerStamp()){
                            tvPingResult.setText("Time synced.");
                            long timeStamp = serverStamp + System.currentTimeMillis() - baseStamp;
                            et_timeStamp.setText(String.valueOf(timeStamp));
                        } else {
                            et_timeStamp.setText(String.valueOf(intentMessage.getTimeStamp()));
                        }
                    }
                    if (!intentMessage.getMac().equals("666")){
                        tv_mac.setText(intentMessage.getMac());
                    }
                    /****
                     * NO,1
                     */
//                    et_id.setText("2122258474");
//                    et_speed.setText("2.69758");
//                    et_ace.setText("0.02787");
//                    et_lat.setText("29.598349");
//                    et_lon.setText("106.295225");
//                    et_direction.setText("106.47");
                    /****
                     * No,2
                     */
                    et_id.setText("-801736957");
                    et_speed.setText("1.48688");
                    et_ace.setText("-0.01");
                    et_lat.setText("28.598280");
                    et_lon.setText("106.295269");
                    et_direction.setText("-74.463");
                    break;
                case MainActivity.ACTION_SEND_MESSAGE:

                    break;
                default:
                    break;
            }
        }
    }

    private void addLogToDB(String context){
        long timeStamp = System.currentTimeMillis();
        SimpleDateFormat simpleDateFormat = new SimpleDateFormat("yyyy-MM-dd-HH:mm:ss");
        Date date = new Date(System.currentTimeMillis());
        String data = simpleDateFormat.format(date);
        Intent intent = new Intent(MainActivity.this, DBTool.class);
        intent.setAction(DBTool.ACTION_ADD);
        intent.putExtra(DBTool.TABLE_NAME, DBTool.TABLE_LOG);
        intent.putExtra(DBTool.LOG_TIMESTAMP, timeStamp);
        intent.putExtra(DBTool.LOG_DATA, data);
        intent.putExtra(DBTool.LOG_CONTEXT, context);
        startService(intent);
    }

    private  void addResultToDB(int id, int time, double distance, boolean warning, long receiverTimeStamp, long sendTimeStamp, long delay){
        Intent intent = new Intent(MainActivity.this, DBTool.class);
        intent.setAction(DBTool.ACTION_ADD);
        intent.putExtra(DBTool.TABLE_NAME, DBTool.TABLE_RESULT);
        intent.putExtra(DBTool.RESULT_ID, id);
        intent.putExtra(DBTool.RESULT_TIME, time);
        intent.putExtra(DBTool.RESULT_DISTANCE, distance);
        intent.putExtra(DBTool.RESULT_WARNING, warning);
        intent.putExtra(DBTool.RESULT_RECEIVERTIMESTAMP, receiverTimeStamp);
        intent.putExtra(DBTool.RESULT_SENDTIMESTAMP, sendTimeStamp);
        intent.putExtra(DBTool.RESULT_DELAY, delay);
        startService(intent);
    }

    private void addMessageToDB(){
        String id = et_id.getEditableText().toString();//将输入的格式统一化为string型
        String timeStamp = et_timeStamp.getEditableText().toString();
        String speed = et_speed.getEditableText().toString();
        String direction = et_direction.getEditableText().toString();
        String lat = et_lat.getEditableText().toString();
        String lon = et_lon.getEditableText().toString();
        String ace = et_ace.getEditableText().toString();
        String mac = tv_mac.getText().toString();

        Intent intent = new Intent(MainActivity.this, DBTool.class);
        intent.setAction(DBTool.ACTION_ADD);
        intent.putExtra(DBTool.TABLE_NAME, DBTool.TABLE_MESSAGE);
        if (id .equals("")){
            intent.putExtra(DBTool.MESSAGE_ID, 666);
        } else {
            intent.putExtra(DBTool.MESSAGE_ID, Integer.parseInt(id));
        }
        if (timeStamp.equals("")){
            intent.putExtra(DBTool.MESSAGE_TIMESTAMP, 666);
        } else {
            intent.putExtra(DBTool.MESSAGE_TIMESTAMP, Long.parseLong(timeStamp));
        }
        if (speed.equals("")){
            intent.putExtra(DBTool.MESSAGE_SPEED, 666);
        } else{
            intent.putExtra(DBTool.MESSAGE_SPEED, Float.parseFloat(speed));
        }
        if (direction.equals("")){
            intent.putExtra(DBTool.MESSAGE_DIRECTION, 666);
        } else {
            intent.putExtra(DBTool.MESSAGE_DIRECTION, Float.parseFloat(direction));
        }
        if (lat.equals("")){
            intent.putExtra(DBTool.MESSAGE_LAT, 666);
        } else {
            intent.putExtra(DBTool.MESSAGE_LAT, Double.parseDouble(lat));
        }
        if (lon.equals("")){
            intent.putExtra(DBTool.MESSAGE_LON, 666);
        } else {
            intent.putExtra(DBTool.MESSAGE_LON, Double.parseDouble(lon));
        }
        if (ace.equals("")){
            intent.putExtra(DBTool.MESSAGE_ACE, 666);
        } else {
            intent.putExtra(DBTool.MESSAGE_ACE, Double.parseDouble(ace));
        }
        if (mac.equals("")){
            intent.putExtra(DBTool.MESSAGE_MAC, "666");
        } else {
            intent.putExtra(DBTool.MESSAGE_MAC, mac);
        }
        startService(intent);
    }

    private void sendMessage(){
        String id = et_id.getEditableText().toString();//将输入的格式统一化为string型
        String timeStamp = et_timeStamp.getEditableText().toString();
        String speed = et_speed.getEditableText().toString();
        String direction = et_direction.getEditableText().toString();
        String lat = et_lat.getEditableText().toString();
        String lon = et_lon.getEditableText().toString();
        String ace = et_ace.getEditableText().toString();

        Message message = new Message();
        message.setId(Integer.valueOf(id));//将上面得到的字符串类型的数据转化为具体相对应的类型
        message.setTimeStamp(Long.valueOf(timeStamp));
        if (!speed.equals("")){
            message.setSpeed(Float.valueOf(speed));
        }
        if (!direction.equals("")){
            message.setDirection(Float.valueOf(direction));
        }
        if (!lat.equals("")){
            message.setLat(Double.valueOf(lat));
        }
        if (!lon.equals("")){
            message.setLon(Double.valueOf(lon));
        }
        if (!ace.equals("")){
            message.setAce(Double.valueOf(ace));
        }
        message.setType(Message.TYPE_MESSAGE);   //普通消息
        //现在Message里面已经有对应格式的数据，接下来是将数据转化为json格式。


        Intent intent = new Intent(MainActivity.this, SendService.class);//跳转到SendService活动
        intent.setAction(SendService.ACTION_SEND_JSON);//将执行服务的活动，现在并不执行，只是告诉android，我们要调用哪个功能。
        intent.putExtra(SendService.EXTRAS_HOST,getServer_Add());//将执行服务活动的限制，IP地址，端口号，还有
        intent.putExtra(SendService.EXTRAS_PORT,SERVER_PORT);
        intent.putExtra(SendService.EXTRAS_JSON,message);
        startService(intent);//现在真正执行服务

    }

    /************************************
     *  发送时间同步消息
     */
    private void sendTimeStampMessage(){
        String id = et_id.getEditableText().toString();//将输入的格式统一化为string型
        String timeStamp = et_timeStamp.getEditableText().toString();
        String speed = et_speed.getEditableText().toString();
        String direction = et_direction.getEditableText().toString();
        String lat = et_lat.getEditableText().toString();
        String lon = et_lon.getEditableText().toString();
        String ace = et_ace.getEditableText().toString();

        Message message = new Message();
        if (!id.equals("")){
            message.setId(Integer.valueOf(id));//将上面得到的字符串类型的数据转化为具体相对应的类型
        }
        if (!timeStamp.equals("")){
            message.setTimeStamp(Long.valueOf(timeStamp));
        }
        if (!speed.equals("")){
            message.setSpeed(Float.valueOf(speed));
        }
        if (!direction.equals("")){
            message.setDirection(Float.valueOf(direction));
        }
        if (!lat.equals("")){
            message.setLat(Double.valueOf(lat));
        }
        if (!lon.equals("")){
            message.setLon(Double.valueOf(lon));
        }
        if (!ace.equals("")){
            message.setAce(Double.valueOf(ace));
        }
        message.setType(Message.TYPE_TIME_SYNC_MESSAGE);   //时间同步消息
        //现在Message里面已经有对应格式的数据，接下来是将数据转化为json格式。

        Intent intent = new Intent(MainActivity.this, SendService.class);//跳转到SendService活动
        intent.setAction(SendService.ACTION_SEND_JSON);//将执行服务的活动，现在并不执行，只是告诉android，我们要调用哪个功能。
        intent.putExtra(SendService.EXTRAS_HOST,getServer_Add());//将执行服务活动的限制，IP地址，端口号，还有
        intent.putExtra(SendService.EXTRAS_PORT,SERVER_PORT);
        intent.putExtra(SendService.EXTRAS_JSON,message);
        startService(intent);//现在真正执行服务
    }

    public void getWarning(){
        img_warning.setImageDrawable(getResources().getDrawable(R.mipmap.ic_danger));

        Uri uri = RingtoneManager.getDefaultUri(RingtoneManager.TYPE_NOTIFICATION);
        Ringtone mRingtone = RingtoneManager.getRingtone(getApplicationContext(),uri);
        mRingtone.play();

        Vibrator vibrator=(Vibrator)getSystemService(Service.VIBRATOR_SERVICE);
        vibrator.vibrate(1000);//震动时长 ms
    }

    public void getSafe(){
        img_warning.setImageDrawable(getResources().getDrawable(R.mipmap.ic_safe));
    }


    /****************************************
     *   发送消息线程
     */
    private class SendThread extends Thread {
        private final static int FREQUENCY = 1000;
        private boolean stop;
        public void stopMe(){
            this.stop = true;
        }
        @Override
        public void run() {
            super.run();
            stop = false;
            while (!stop){
                try {
                    sendMessage();
                    addMessageToDB();
                    sleep(FREQUENCY);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }

}

