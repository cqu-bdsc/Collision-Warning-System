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


    public class RecationBroadcast extends BroadcastReceiver {

        @Override//复写onReceive方法
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            switch (action){
                case ReceiveThread.ACTION_STRING:
                    String message = intent.getExtras().getString(ReceiveThread.STRING_CONTEXT);
                    //tvReceive.setText(message);
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
