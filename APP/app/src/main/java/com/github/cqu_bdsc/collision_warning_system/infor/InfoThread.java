package com.github.cqu_bdsc.collision_warning_system.infor;

import android.content.Context;
import android.content.Intent;

import com.github.cqu_bdsc.collision_warning_system.DAO.Message;

public class InfoThread extends Thread {
    public final static String ACTION_INFORMATION = "ACTION_INFORMATION";

    public final static String EXTRAR_INFORMATION = "EXTRAR_INFORMATION;";

    private final static int FREQUENCY = 100;
    private boolean stop;
    private InfoTool infoTool;
    private Context context;
    public InfoThread(Context context){
        this.context = context;
        infoTool = new InfoTool(context);
    }

    @Override
    public void run() {
        super.run();
        stop = false;
        while (!stop){
            Message infor = infoTool.getInfo();
            sendIntentMessage(infor);//将数据广播出去
            try {
                sleep(FREQUENCY);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    public void sendIntentMessage(Message message){
        Intent intent = new Intent();
        intent.setAction(InfoThread.ACTION_INFORMATION);//告诉android将要执行什么功能
        intent.putExtra(InfoThread.EXTRAR_INFORMATION, message);//信息
        context.sendBroadcast(intent);//广播信息
    }
}
