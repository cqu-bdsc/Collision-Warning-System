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

import com.github.cqu_bdsc.collision_warning_system.udp.ReceiveThread;
import com.github.cqu_bdsc.collision_warning_system.udp.SendService;

import java.io.IOException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class MainActivity extends AppCompatActivity {

    private static final String SERVER_IP = "192.168.1.80";
    private static final String SERVER_PORT = "4040";
    private String Server_Add = "192.168.1.80";

    private BroadcastReceiver broadcastReceiver = null;
    private final IntentFilter intentFilter = new IntentFilter();//这里有问题1

    private Button      btnPing;//以下是可视化程序里面的常量
    private Button      btnSend;
    private EditText    etIp;
    private EditText    textSend;
    private TextView    tvPingResult;
    private TextView    tvReceive;

    @Override
    protected void onResume() {
        super.onResume();
        broadcastReceiver = new RecationBroadcast();
        registerReceiver(broadcastReceiver, intentFilter);
    }

    @Override
    protected void onPause() {
        super.onPause();
        unregisterReceiver(broadcastReceiver);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        intentFilter.addAction(ReceiveThread.ACTION_STRING);

        etIp        = (EditText) findViewById(R.id.et_ipAdd);
        btnPing     = (Button)   findViewById(R.id.btn_testPing);
        tvPingResult= (TextView) findViewById(R.id.tvPingResult);

        textSend    = (EditText) findViewById(R.id.textSend);
        btnSend     = (Button)   findViewById(R.id.btn_send);
        tvReceive = (TextView) findViewById(R.id.tv_receive);

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
            public void onClick(View view) {
                String strSend = (String) textSend.getEditableText().toString();
                Intent intent = new Intent(MainActivity.this, SendService.class);
                intent.setAction(SendService.ACTION_SEND_STRING);
                intent.putExtra(SendService.EXTRAS_HOST,getServer_Add());
                intent.putExtra(SendService.EXTRAS_PORT,SERVER_PORT);
                intent.putExtra(SendService.EXTRAS_STRING,strSend);
                startService(intent);

            }
        });


        /**
         * 运行接收线程
         */
        ReceiveThread receiveThread = new ReceiveThread(MainActivity.this, getServer_Add());
        receiveThread.start();

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

        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            switch (action){
                case ReceiveThread.ACTION_STRING:
                    String message = intent.getExtras().getString(ReceiveThread.STRING_CONTEXT);
                    tvReceive.setText(message);
                    break;

                default:
                    break;
            }
        }
    }
}
