package com.github.cqu_bdsc.collision_warning_system.udp;


import android.app.IntentService;
import android.content.Context;
import android.content.Intent;
import android.support.annotation.Nullable;

import com.github.cqu_bdsc.collision_warning_system.DAO.Message;

import org.json.JSONObject;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;

public class SendService extends IntentService  {//继承父类IntentService

    public static final int BUFF_SIZE = 8192;//最大数据长度

    public static final String ACTION_SEND_STRING = "SEND_STRING";
    public static final String ACTION_SEND_JSON = "SEND_JSON";

    public static final String EXTRAS_HOST      = "EXTRAS_HOST";//主机
    public static final String EXTRAS_PORT      = "EXTRAS_PORT";//端口
    public static final String EXTRAS_STRING    = "EXTRAS_STRING";//在子类中定义常量
    public static final String EXTRAS_JSON        = "EXTRAS_JSON";

    public SendService(String name){
        super(name);
    }
    public SendService(){
        super("SendService");
    }
    @Override
    protected void onHandleIntent(@Nullable Intent intent) {
        Context context = getApplicationContext();

        /**
         * 发送字符串
         */
        try {
            String action = (String) intent.getAction();
            String host = intent.getExtras().getString(EXTRAS_HOST);
            String port = intent.getExtras().getString(EXTRAS_PORT);
            InetAddress serverAddr = InetAddress.getByName(host);
            //InetAddress getAddress()   返回某台机器的IP地址，此数据报是将要发往该主机或是从该主机接收到的

            DatagramSocket udpSocket = null;
            DatagramPacket packet = null;


            /**
             *  发送字符串
             */
            if (action.equals(ACTION_SEND_STRING)){


                String extras_string = intent.getExtras().getString(EXTRAS_STRING);
                try {
                    if (udpSocket == null){
                        udpSocket = new DatagramSocket();
                    }
                    byte[] buff = (byte[]) extras_string.getBytes();

                    packet = new DatagramPacket(buff, buff.length,serverAddr,Integer.valueOf(port));
                    udpSocket.send(packet);
                } catch (SocketException e) {
                    e.printStackTrace();
                }  finally {
                    if (udpSocket != null){
                        udpSocket.close();
                    }
                }




            } else if (action.equals(SendService.ACTION_SEND_JSON)){
                Message message = (Message) intent.getExtras().get(EXTRAS_JSON);
                JSONObject jsonObject = message.toJSON();
                try {
                    if (udpSocket == null){
                        udpSocket = new DatagramSocket();
                    }
                    byte[] buff = (byte[]) jsonObject.toString().getBytes("UTF-8");

                    packet = new DatagramPacket(buff, buff.length,serverAddr,Integer.valueOf(port));
                    //构造数据包，用来将length长度的数据包发送到指定主机（指定mac地址的主机）上的指定端口号
                    udpSocket.send(packet);
                } catch (SocketException e) {
                    e.printStackTrace();
                }  finally {
                    if (udpSocket != null){
                        udpSocket.close();
                    }
                }
            }
             else {
            }
        } catch (UnknownHostException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }


    }

}
