package com.github.cqu_bdsc.collision_warning_system.DAO;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.Serializable;

public class Result implements Serializable {
    public static int ERROR_VALUE = -666;
    private int id;
    private int time;
    private double distance;
    private  boolean warning;
    private int type ; // // type=1 为时间同步消息
    //type = 0 为普通消息
    private long timeStamp1;
    private long receiverTimeStamp;
    private long sendTimeStamp;

    public Result(){
        id = ERROR_VALUE;
        time = ERROR_VALUE;
        distance = ERROR_VALUE;
        warning = false;
        timeStamp1 = ERROR_VALUE;
        receiverTimeStamp = ERROR_VALUE;
        sendTimeStamp = ERROR_VALUE;
        type = ERROR_VALUE;
    }

    public int getType() {
        return type;
    }

    public void setType(int type) {
        this.type = type;
    }

    public long getTimeStamp1() {
        return timeStamp1;
    }

    public void setTimeStamp1(long timeStamp1) {
        this.timeStamp1 = timeStamp1;
    }

    public long getReceiverTimeStamp() {
        return receiverTimeStamp;
    }

    public void setReceiverTimeStamp(long receiverTimeStamp) {
        this.receiverTimeStamp = receiverTimeStamp;
    }

    public long getSendTimeStamp() {
        return sendTimeStamp;
    }

    public void setSendTimeStamp(long sendTimeStamp) {
        this.sendTimeStamp = sendTimeStamp;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getTime() {
        return time;
    }

    public void setTime(int time) {
        this.time = time;
    }

    public double getDistance() {
        return distance;
    }

    public void setDistance(double distance) {
        this.distance = distance;
    }

    public boolean isWarning() {
        return warning;
    }

    public void setWarning(boolean warning) {
        this.warning = warning;
    }

    public void fromJSON(JSONObject jsonObject){
        try {
            if (2 == jsonObject.getInt("type")){
                this.setId(jsonObject.getInt("id"));
                this.setType(jsonObject.getInt("type"));
                this.setTime(jsonObject.getInt("time"));
                this.setDistance(jsonObject.getDouble("distance"));
                this.setWarning(jsonObject.getBoolean("warning"));
            } else if (3 == jsonObject.getInt("type")){
                this.setId(jsonObject.getInt("id"));
                this.setType(jsonObject.getInt("type"));
                this.setTimeStamp1(jsonObject.getLong("timeStamp1"));
                this.setReceiverTimeStamp(jsonObject.getLong("receiverTimeStamp"));
                this.setSendTimeStamp(jsonObject.getLong("sendTimeStamp"));
            } else {

            }
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }
}
