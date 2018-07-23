package com.github.cqu_bdsc.collision_warning_system;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.Serializable;

public class Message implements Serializable {
    int id;
    int timeStamp;
    double speed;
    int direction;
    double lat;
    double lon;
    double ace;

    Message(){
        id = 0;
        timeStamp = 0;
        speed = 0;
        direction = 0;
        lat = 0;
        lon = 0;
        ace = 0;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getId() {
        return id;
    }

    public void setTimeStamp(int timeStamp) {
        this.timeStamp = timeStamp;
    }

    public int getTimeStamp() {
        return timeStamp;
    }

    public void setDirection(int direction) {
        this.direction = direction;
    }

    public int getDirection() {
        return direction;
    }

    public void setSpeed(double speed) {
        this.speed = speed;
    }

    public double getSpeed() {
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
