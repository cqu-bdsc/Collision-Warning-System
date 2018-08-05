package com.github.cqu_bdsc.collision_warning_system.infor;
import android.annotation.SuppressLint;
import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.util.Log;

import com.github.cqu_bdsc.collision_warning_system.DAO.Message;
import com.github.cqu_bdsc.collision_warning_system.MainActivity;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.Reader;
import java.io.StringWriter;
import java.io.Writer;
import java.net.NetworkInterface;
import java.text.SimpleDateFormat;
import java.util.Collections;
import java.util.Date;
import java.util.List;
import java.util.TimeZone;

public class InfoTool {
    public static final String TAG = "InfoTool";

    private Message message;
    private WifiManager wifiManager;
    private SensorManager sensorManager;

    private static final String marshmallowMacAddress = "02:00:00:00:00:00";
    private static final String fileAddressMac = "/sys/class/net/wlan0/address";

    private Context context;
    /**
     * accelerometer    加速度传感器
     * magnetic_field   磁场传感器
     *
     */
    private Sensor accelerometer;
    private Sensor magnetic_field;

    private float[] accelerometerValues = new float[3];
    private float[] magneticFieldValues = new float[3];

    public InfoTool(Context context) {
        init(context);
    }

    /**
     * 初始化
     */
    @SuppressLint("WifiManagerLeak")
    private void init(Context context) {
        this.context = context;
        message = new Message();
        wifiManager = (WifiManager) context.getSystemService(Context.WIFI_SERVICE);//对象初始化

        sensorManager = (SensorManager) context.getSystemService(Context.SENSOR_SERVICE);
        /**
         * 初始传感器
         */
        accelerometer = sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        magnetic_field = sensorManager.getDefaultSensor(Sensor.TYPE_MAGNETIC_FIELD);

        sensorManager.registerListener(new MySensorEventListener(InfoTool.this),//注册
                accelerometer, Sensor.TYPE_ACCELEROMETER);
        sensorManager.registerListener(new MySensorEventListener(InfoTool.this),
                magnetic_field, Sensor.TYPE_MAGNETIC_FIELD);

    }

    /**
     * 注销服务
     */
    public void uninit(){
        sensorManager.unregisterListener(new MySensorEventListener(InfoTool.this));
        resetInfor();
    }

    public Message getInfo(){
        /**
         * 获取信息
         * 并通知Activity 进行显示
         */
        getMac();                           //获得MAC地址
        getTimeNow();                       //获得时间
        calculateOrientation();             //获得方向
        return message;
    }

    private void resetInfor() {
        message = new Message();
    }

    /**
     * 获取时间
     * 获取更精确时间，精确到毫秒
     */
    private void getTimeNow(){
        message.setTimeStamp(System.currentTimeMillis());
    }

    /**
     * 计算方向
     * 0为正北
     * 90为正东
     * -90为正西
     * 180或-180为正南
     * @return
     */
    private boolean calculateOrientation() {
        boolean isSuccess = true;
        float[] values = new float[3];
        float[] R = new float[9];
        SensorManager.getRotationMatrix(R, null, accelerometerValues,
                magneticFieldValues);
        SensorManager.getOrientation(R, values);
        values[0] = (float) Math.toDegrees(values[0]);
        message.setDirection(values[0]);
        return isSuccess;
    }

    private class MySensorEventListener implements SensorEventListener {
        private float gravity[] = {0,0,0};
        private float linear_acceleration[] = {0,0,0};
        private InfoTool infoTool;
        public MySensorEventListener(InfoTool infoTool){
            this.infoTool = infoTool;
        }

        @Override
        public void onSensorChanged(SensorEvent event) {
            // TODO Auto-generated method stub
            if (event.sensor.getType() == Sensor.TYPE_ACCELEROMETER) {
                accelerometerValues = event.values;
                final float alpha = (float) 0.8;

                //gravity[0] = alpha * gravity[0] + (1 - alpha) * event.values[0];
                gravity[1] = alpha * gravity[1] + (1 - alpha) * event.values[1];
                //gravity[2] = alpha * gravity[2] + (1 - alpha) * event.values[2];

                //linear_acceleration[0] = event.values[0] - gravity[0];
                linear_acceleration[1] = event.values[1] - gravity[1];
                //linear_acceleration[2] = event.values[2] - gravity[2];

                infoTool.message.setAce(linear_acceleration[1]);

            }
            if (event.sensor.getType() == Sensor.TYPE_MAGNETIC_FIELD) {
                magneticFieldValues = event.values;
            }
            calculateOrientation();
        }

        @Override
        public void onAccuracyChanged(Sensor sensor, int accuracy) {
            // TODO Auto-generated method stub

        }

    }


    /**
     * 获取MAC地址，并根据MAC地址的哈希值获得设备号
     */
    private void getMac(){
        String macAddress = null;
        WifiInfo wifiInfo = wifiManager.getConnectionInfo();
        if (wifiInfo != null &&
                marshmallowMacAddress.equals(wifiInfo.getMacAddress())){
            try {
                macAddress = getAdressMacByInterface();
                if (macAddress != null){

                }else {
                    macAddress = getAddressMacByFile(wifiManager);
                }
            }catch (Exception e){
                e.printStackTrace();
            }
        }else {
            if (wifiInfo != null && wifiInfo.getMacAddress() != null){
                macAddress = wifiInfo.getMacAddress();
            }else {
                macAddress = marshmallowMacAddress;
            }
        }
        int deviceNo = macAddress.hashCode();
        message.setId(deviceNo);
    }

    private static String getAdressMacByInterface(){
        try {
            List<NetworkInterface> all = Collections.list(NetworkInterface.getNetworkInterfaces());
            for (NetworkInterface nif : all) {
                if (nif.getName().equalsIgnoreCase("wlan0")) {
                    byte[] macBytes = nif.getHardwareAddress();
                    if (macBytes == null) {
                        return "";
                    }

                    StringBuilder res1 = new StringBuilder();
                    for (byte b : macBytes) {
                        res1.append(String.format("%02X:",b));
                    }

                    if (res1.length() > 0) {
                        res1.deleteCharAt(res1.length() - 1);
                    }
                    return res1.toString();
                }
            }

        } catch (Exception e) {
            Log.e("MobileAcces", "Erreur lecture propriete Adresse MAC ");
        }
        return null;
    }

    private static String getAddressMacByFile(WifiManager wifiMan) throws Exception {
        String ret;
        int wifiState = wifiMan.getWifiState();

        wifiMan.setWifiEnabled(true);
        File fl = new File(fileAddressMac);
        FileInputStream fin = new FileInputStream(fl);
        ret = crunchifyGetStringFromStream(fin);
        fin.close();

        boolean enabled = WifiManager.WIFI_STATE_ENABLED == wifiState;
        wifiMan.setWifiEnabled(enabled);
        return ret;
    }

    private static String crunchifyGetStringFromStream(InputStream crunchifyStream) throws IOException {
        if (crunchifyStream != null) {
            Writer crunchifyWriter = new StringWriter();

            char[] crunchifyBuffer = new char[2048];
            try {
                Reader crunchifyReader = new BufferedReader(new InputStreamReader(crunchifyStream, "UTF-8"));
                int counter;
                while ((counter = crunchifyReader.read(crunchifyBuffer)) != -1) {
                    crunchifyWriter.write(crunchifyBuffer, 0, counter);
                }
            } finally {
                crunchifyStream.close();
            }
            return crunchifyWriter.toString();
        } else {
            return "No Contents";
        }
    }

}