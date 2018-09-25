package com.github.cqu_bdsc.collision_warning_system.database;

import com.raizlabs.android.dbflow.annotation.Column;
import com.raizlabs.android.dbflow.annotation.PrimaryKey;
import com.raizlabs.android.dbflow.annotation.Table;
import com.raizlabs.android.dbflow.structure.BaseModel;

@Table(database = AppDataBase.class)
public class MessageModel extends BaseModel {

    @PrimaryKey(autoincrement = true)
    private int messageID;

    @Column
    private int id;

    @Column
    private long timeStamp;

    @Column
    private float speed;

    @Column
    private float direction;

    @Column
    private double lat;

    @Column
    private double lon;

    @Column
    private double ace;

    @Column
    private String mac;

    public void setMessageID(int messageID) {
        this.messageID = messageID;
    }

    public int getMessageID() {
        return messageID;
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
}
