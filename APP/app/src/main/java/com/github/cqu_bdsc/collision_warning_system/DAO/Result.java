package com.github.cqu_bdsc.collision_warning_system.DAO;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.Serializable;

public class Result implements Serializable {
    /*****************************************
     * When type = TYPE_MESSAGE
     * there are some value is not null:
     * id, warning, sendTimeStamp(Server send), receiverTimeStamp(Android  receive)
     * ****************************************************************************
     *
     * When type = TYPT_TIME_SYNC_RESULT
     * there are some value is not null:
     * id , timeStamp(Android send Fist), receiverTimeStamp(Server receive),
     * sendTimeStamp(Server send), And  currentTimeStamp(Android receive now)
     *
     * It is how the time sync works,
     * When the android  send  a  time_sync_type message to server,
     * Server resend  a receiverTimeStamp and  a  sendTimeStamp to  android,
     * then android could get the delay of the transmission
     *
     *                 Android             Server
     *    T1          timeStamp
     *    T2                          receiveTimeStamp
     *    T3                           sendTimeStamp
     *    T4       currentTimeStamp
     *    So, total delay is  (currentTimeStamp - timeStamp) - (sendTimeStamp - receiverTimeStamp) = ANS
     *    Then, half of it is ANS/2
     *
     *    AND
     *                  Android                   Server
     *    T4       currentTimeStamp      sendTimeStamp+ ANS/2)= baseTimeStamp
     *    T5          nowTimeStamp       baseTimeStamp+ (nowTimeStamp - currentTimeStamp)
     *
     *****************************************/

    public static final String TYPE_RESULT = "TYPE_RESULT";
    public static final String TYPE_TIME_SYNC_RESULT = "TYPE_TIME_SYNC_RESULT";
    public static int ERROR_VALUE = -666;
    private int id;
    private int time;
    private double distance;
    private  boolean warning;
    private String type;
    private long timeStamp;
    private long receiverTimeStamp;
    private long sendTimeStamp;

    public Result(){
        id = ERROR_VALUE;
        time = ERROR_VALUE;
        distance = ERROR_VALUE;
        warning = false;
        timeStamp = ERROR_VALUE;
        receiverTimeStamp = ERROR_VALUE;
        sendTimeStamp = ERROR_VALUE;
        type = "666";
    }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }

    public long getTimeStamp() {
        return timeStamp;
    }

    public void setTimeStamp(long timeStamp1) {
        this.timeStamp = timeStamp1;
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
                this.setId(jsonObject.getInt("id"));
                this.setType(jsonObject.getString("type"));
                this.setTime(jsonObject.getInt("time"));
                this.setDistance(jsonObject.getDouble("distance"));
                this.setWarning(jsonObject.getBoolean("warning"));
                this.setTimeStamp(jsonObject.getLong("timeStamp"));
                this.setReceiverTimeStamp(jsonObject.getLong("receiverTimeStamp"));
                this.setSendTimeStamp(jsonObject.getLong("sendTimeStamp"));
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }
}
