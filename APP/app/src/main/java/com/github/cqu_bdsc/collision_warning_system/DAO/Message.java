package com.github.cqu_bdsc.collision_warning_system.DAO;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.Serializable;

public class Message implements Serializable {
    public static int ERROR_VALUE = -666;
    private int id;
    private long timeStamp;
    private float speed;
    private float direction;
    private double lat;
    private double lon;
    private double ace;
    private String mac;

   public   Message(){
        id = ERROR_VALUE;
        timeStamp = ERROR_VALUE;
        speed = ERROR_VALUE;
        direction = ERROR_VALUE;
        lat = ERROR_VALUE;
        lon = ERROR_VALUE;
        ace = ERROR_VALUE;
        mac = "666";
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getId() {
        return id;
    }

    public void setTimeStamp(long timeStamp) {
        this.timeStamp = timeStamp;
    }

    public long getTimeStamp() {
        return timeStamp;
    }

    public void setDirection(float direction) {
        this.direction = direction;
    }

    public float getDirection() {
        return direction;
    }

    public void setSpeed(float speed) {
        this.speed = speed;
    }

    public float getSpeed() {
        return speed;
    }

    public void setLat(double lat) {
        this.lat = lat;
    }

    public double getLat() {
        return lat;
    }

    public void setLon(double lon) {
        this.lon = lon;
    }

    public double getLon() {
        return lon;
    }

    public void setAce(double ace) {
        this.ace = ace;
    }

    public double getAce() {
        return ace;
    }

    public String getMac() {
        return mac;
    }

    public void setMac(String mac) {
        this.mac = mac;
    }

    public JSONObject toJSON(){

        try {
            JSONObject jsonObject = new JSONObject();
            jsonObject.put("id", String.valueOf(getId()));
            jsonObject.put("timeStamp",String.valueOf(getTimeStamp()));
            jsonObject.put("speed",String.valueOf(getSpeed()));
            jsonObject.put("direction",String.valueOf(getDirection()));
            jsonObject.put("lat",String.valueOf(getLat()));
            jsonObject.put("lon",String.valueOf(getLon()));
            jsonObject.put("acc",String.valueOf(getAce()));
            return jsonObject;
        } catch (JSONException e) {
            e.printStackTrace();
            return null;
        }
    }
}
