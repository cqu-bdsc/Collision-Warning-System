package com.github.cqu_bdsc.collision_warning_system.udp;


import android.app.IntentService;
import android.content.Context;
import android.content.Intent;
import android.support.annotation.Nullable;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;

public class SendService extends IntentService {

    public static final int BUFF_SIZE = 8192;

    public static final String ACTION_SEND_STRING = "SEND_STRING";

    public static final String EXTRAS_HOST      = "EXTRAS_HOST";
    public static final String EXTRAS_PORT      = "EXTRAS_PORT";
    public static final String EXTRAS_STRING    = "EXTRAS_STRING";

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

            DatagramSocket udpSocket = null;
            DatagramPacket packet = null;


            /**
             *  发送字符串
             */
            if (action.equals(ACTION_SEND_STRING)){
                udpSocket = new DatagramSocket(Integer.valueOf(port));
                //发送UDP报文类型
                try {
                    String sendType = ReceiveThread.TYPE_STRING;
                    packet = new DatagramPacket(sendType.getBytes(),sendType.getBytes().length,serverAddr,Integer.valueOf(port));
                    udpSocket.send(packet);
                } catch (SocketException e) {
                    e.printStackTrace();
                }finally {
                    if (udpSocket != null){
                        udpSocket.close();
                    }
                }

                String extras_string = intent.getExtras().getString(EXTRAS_STRING);
                try {
                    udpSocket = new DatagramSocket(Integer.valueOf(port));
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


                /**
                 * 发送终止符
                 */
                try {
                    String end = ReceiveThread.TYPE_END;
                    udpSocket = new DatagramSocket(Integer.valueOf(port));
                    packet = new DatagramPacket(end.getBytes(),end.getBytes().length,serverAddr,Integer.valueOf(port));
                    udpSocket.send(packet);
                } catch (SocketException e) {
                    e.printStackTrace();
                }finally {
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
