package com.github.cqu_bdsc.collision_warning_system.udp;

import android.content.Context;
import android.content.Intent;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;

public class ReceiveThread extends Thread {

    private Context context;

    public static final String ACTION_STRING = "ACTION_STRING";
    public static final String STRING_CONTEXT = "STRING_CONTEXT";

    private static final String SERVER_IP = "192.168.1.80";
    private static final String RECEIVE_PORT = "4040";

    public static final String TYPE_STRING = "TYPE_STRING";
    public static final String TYPE_PICTURE = "TYPE_PICTURE";
    public static final String TYPE_END = "!end;";

    private String serverIp = null;//流，可以装东西

    public ReceiveThread(){
        this.serverIp = SERVER_IP;
    }
    public ReceiveThread(Context context,String serverIp){
        this.context = context;
        this.serverIp = serverIp;
    }

    private ByteArrayOutputStream baos = new ByteArrayOutputStream();

    @Override
    public void run() {//线程的main函数

        try {
            DatagramPacket reveivePacket;
            DatagramSocket reveiveSocket;

            reveiveSocket = new DatagramSocket(Integer.valueOf(RECEIVE_PORT));//把接收端口号的字符串变成整数
            byte[] buff = new byte[SendService.BUFF_SIZE];//发送过来的数据的长度范围

            String dataType = null;//先不限定数据的类型
            while (true){
                reveivePacket = new DatagramPacket(buff, buff.length);

                try {//try和catch合作，如果出错可以马上看到是哪个地方的错误
                    reveiveSocket.receive(reveivePacket);
                } catch (IOException e) {
                    e.printStackTrace();//IO即输入输出错误，错误地方用e来表示，在哪里出错就在哪里打印出e
                }

                String msg = new String(reveivePacket.getData(),0,reveivePacket.getLength());


                if (msg.startsWith(TYPE_STRING)){
                    dataType = TYPE_STRING;

                } else if (msg.startsWith(TYPE_PICTURE)){
                    dataType = TYPE_PICTURE;

                } else if (msg.startsWith(TYPE_END)){//如输入结束，就需要清空数据流中的信息
                    // 处理BAOS 数据
                    ByteArrayOutputStream byteArrayOutputStream = baos;

                    HandleData(dataType, byteArrayOutputStream);

                    baos.reset();//重置baos
                } else {

                    baos.write(reveivePacket.getData(),0,reveivePacket.getLength());//将新数据加入到数据流中，并把它打印出来
                }

            }


        } catch (SocketException e) {
            e.printStackTrace();
        }


    }

    private void HandleData(String dataType, ByteArrayOutputStream byteArrayOutputStream){//处理数据函数
        byte[] bytes = byteArrayOutputStream.toByteArray();

        if (dataType.equals(TYPE_STRING)){
            String message = new String(bytes);
            System.out.print(message);
            SendIntent(ReceiveThread.ACTION_STRING, message);

        } else {

        }
    }


    private void SendIntent(String action, String something){
        Intent intent = new Intent();
        if (action.equals(ReceiveThread.ACTION_STRING)){
            intent.setAction(ReceiveThread.ACTION_STRING);
            intent.putExtra(ReceiveThread.STRING_CONTEXT, something);
            context.sendBroadcast(intent);
        }  else {

        }
    }
}

