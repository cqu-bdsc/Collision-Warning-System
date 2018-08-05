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

   public   Message(){
        id = ERROR_VALUE;
        timeStamp = ERROR_VALUE;
        speed = ERROR_VALUE;
        direction = ERROR_VALUE;
        lat = ERROR_VALUE;
        lon = ERROR_VALUE;
        ace = ERROR_VALUE;
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

    public JSONObject toJSON(){

        try {
            JSONObject jsonObject = new JSONObject();
            jsonObject.put("id", getId());
            jsonObject.put("timeStamp",getTimeStamp());
            jsonObject.put("speed",getSpeed());
            jsonObject.put("direction",getDirection());
            jsonObject.put("lat",getLat());
            jsonObject.put("lon",getLon());
            jsonObject.put("acc",getAce());
            return jsonObject;
        } catch (JSONException e) {
            e.printStackTrace();
            return null;
        }
    }
}
