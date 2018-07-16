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

    private String serverIp = null;

    public ReceiveThread(){
        this.serverIp = SERVER_IP;
    }
    public ReceiveThread(Context context,String serverIp){
        this.context = context;
        this.serverIp = serverIp;
    }

    private ByteArrayOutputStream baos = new ByteArrayOutputStream();

    @Override
    public void run() {

        try {
            DatagramPacket reveivePacket;
            DatagramSocket reveiveSocket;

            reveiveSocket = new DatagramSocket(Integer.valueOf(RECEIVE_PORT));
            byte[] buff = new byte[SendService.BUFF_SIZE];

            String dataType = null;
            while (true){
                reveivePacket = new DatagramPacket(buff, buff.length);

                try {
                    reveiveSocket.receive(reveivePacket);
                } catch (IOException e) {
                    e.printStackTrace();
                }

                String msg = new String(reveivePacket.getData(),0,reveivePacket.getLength());


                if (msg.startsWith(TYPE_STRING)){
                    dataType = TYPE_STRING;

                } else if (msg.startsWith(TYPE_PICTURE)){
                    dataType = TYPE_PICTURE;

                } else if (msg.startsWith(TYPE_END)){
                    // 处理BAOS 数据
                    ByteArrayOutputStream byteArrayOutputStream = baos;

                    HandleData(dataType, byteArrayOutputStream);

                    baos.reset();
                } else {

                    baos.write(reveivePacket.getData(),0,reveivePacket.getLength());
                }

            }


        } catch (SocketException e) {
            e.printStackTrace();
        }


    }

    private void HandleData(String dataType, ByteArrayOutputStream byteArrayOutputStream){
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

