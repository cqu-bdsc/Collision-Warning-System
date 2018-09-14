package com.github.cqu_bdsc.collision_warning_system.database;

import android.app.IntentService;
import android.content.Intent;
import android.support.annotation.Nullable;

import com.github.cqu_bdsc.collision_warning_system.DAO.Message;
import com.github.cqu_bdsc.collision_warning_system.MainActivity;
import com.github.cqu_bdsc.collision_warning_system.udp.ReceiveThread;
import com.raizlabs.android.dbflow.sql.language.SQLite;
import com.raizlabs.android.dbflow.sql.language.Select;

import java.util.List;

public class DBTool extends IntentService {

    /**
     * Creates an IntentService.  Invoked by your subclass's constructor.
     *
     * @param name Used to name the worker thread, important only for debugging.
     */
    public DBTool(String name) {
        super(name);
    }
    public DBTool(){
        super("DBTool");
    }

    public static final String ACTION_ADD       = "ACTION_ADD";
    public static final String ACTION_DELETE    = "ACTION_DELETE";
    public static final String ACTION_SELETE    = "ACTION_SELETE";

    public static final String TABLE_NAME               = "TABLE_NAME";
    public static final String TABLE_MESSAGE            = "TABLE_MESSAGE";
    public static final String TABLE_RESULT             = "TABLE_RESULT";
    public static final String TABLE_ALL                = "TABLE_ALL";

    /*MessageModel*/
    public static final String MESSAGE_ID   = "MESSAGE_ID";
    public static final String MESSAGE_TIMESTAMP = "MESSAGE_TIMESTAMP";
    public static final String MESSAGE_SPEED = "MESSAGE_SPEED";
    public static final String MESSAGE_DIRECTION = "MESSAGE_DIRECTION";
    public static final String MESSAGE_LAT = "MESSAGE_LAT";
    public static final String MESSAGE_LON = "MESSAGE_LON";
    public static final String MESSAGE_ACE = "MESSAGE_ACE";
    public static final String MESSAGE_MAC = "MESSAGE_MAC";
    public static final String MESSAGE_TYPE = "MESSAGE_TYPE";

    /*ResultModel*/
    public static final String RESULT_ID = "RESULT_ID";
    public static final String RESULT_TIME = "RESULT_TIME";
    public static final String RESULT_TYPE = "RESULT_TYPE";
    public static final String RESULT_DISTANCE = "RESULT_DISTANCE";
    public static final String RESULT_WARNING = "RESULT_WARNING";
    public static final String RESULT_TIMESTAMP1 = "RESULT_TIMESTAMP1";
    public static final String RESULT_RECEIVERTIMESTAMP = "RESULT_RECEIVERTIMESTAMP";
    public static final String RESULT_SENDTIMESTAMP = "RESULT_SENDTIMESTAMP";
    public static final String RESULT_TIMESTAMP2 = "RESULT_TIMESTAMP2";

    @Override
    protected void onHandleIntent(@Nullable Intent intent) {
        String action = intent.getAction();
        /**
         * 添加操作
         */
        if (action.equals(DBTool.ACTION_ADD)){
            String tableName = intent.getExtras().getString(DBTool.TABLE_NAME);
            if (tableName.equals(DBTool.TABLE_RESULT)){
                int id = intent.getExtras().getInt(RESULT_ID);
                int time = intent.getExtras().getInt(RESULT_TIME);
                double distance = intent.getExtras().getDouble(RESULT_DISTANCE);
                boolean warning = intent.getExtras().getBoolean(RESULT_WARNING);
                int type = intent.getExtras().getInt(RESULT_TYPE);
                long timeStamp1 = intent.getExtras().getLong(RESULT_TIMESTAMP1);
                long receiverTimeStamp = intent.getExtras().getLong(RESULT_RECEIVERTIMESTAMP);
                long sendTimeStamp = intent.getExtras().getLong(RESULT_SENDTIMESTAMP);
                long timeStamp2 = intent.getExtras().getLong(RESULT_TIMESTAMP2);
                insertResult(id, time,  distance,  warning, type, timeStamp1,  receiverTimeStamp, sendTimeStamp, timeStamp2);
            }else if (tableName.equals(DBTool.TABLE_MESSAGE)){
                int id = intent.getExtras().getInt(DBTool.MESSAGE_ID);
                long timeStamp = intent.getExtras().getLong(DBTool.MESSAGE_TIMESTAMP);
                float speed = intent.getExtras().getFloat(DBTool.MESSAGE_SPEED);
                float direction = intent.getExtras().getFloat(DBTool.MESSAGE_DIRECTION);
                double lat = intent.getExtras().getDouble(DBTool.MESSAGE_LAT);
                double lon = intent.getExtras().getDouble(DBTool.MESSAGE_LON);
                double ace = intent.getExtras().getDouble(DBTool.MESSAGE_ACE);
                String mac = intent.getExtras().getString(DBTool.MESSAGE_MAC);
                int type = intent.getExtras().getInt(DBTool.MESSAGE_TYPE);
                insertMessage(id, timeStamp, speed,  direction,  lat, lon,  ace, mac,  type);
            }else {

            }
        }
        /**
         * 删除操作
         */
        else if (action.equals(DBTool.ACTION_DELETE)){
            String tableName = intent.getExtras().getString(DBTool.TABLE_NAME);
            if (tableName.equals(DBTool.TABLE_MESSAGE)){
                List<MessageModel> messageModels = SQLite.select().from(MessageModel.class).queryList();
                for (MessageModel messageModel:messageModels){
                    messageModel.delete();
                }
            } else if (tableName.equals(DBTool.TABLE_RESULT)){
                List<ResultModel> resultModels = SQLite.select().from(ResultModel.class).queryList();
                for (ResultModel resultModel:resultModels){
                    resultModel.delete();
                }
            } else {

            }
        }
        /**
         * 查询操作
         */
        else if (action.equals(DBTool.ACTION_SELETE)){
            String tableName = intent.getExtras().getString(DBTool.TABLE_NAME);
            if (tableName.equals(DBTool.TABLE_MESSAGE)){
                //返回所有查询结果
                List<MessageModel> messageModels = new Select().from(MessageModel.class).queryList();
                int number = messageModels.size();
                sendLog("Message表有"+String.valueOf(number)+"条记录");

            }else if (tableName.equals(DBTool.TABLE_RESULT)){
                List<ResultModel> resultModels = new Select().from(ResultModel.class).queryList();
                int number = resultModels.size();
                sendLog("Result表有"+String.valueOf(number)+"条记录");
            }else if (tableName.equals(DBTool.TABLE_ALL)){
                List<MessageModel> messageModels = new Select().from(MessageModel.class).queryList();
                int numberMessage = messageModels.size();
                List<ResultModel> resultModels = new Select().from(ResultModel.class).queryList();
                int numberResult = resultModels.size();
                sendSQL("Message表有"+String.valueOf(numberMessage)+"条记录  "+"Result表有"+String.valueOf(numberResult)+"条记录");
            }
        }

    }

    private void insertMessage(int id, long timeStamp, float speed, float direction, double lat, double lon, double ace, String mac, int type){
        MessageModel messageModel = new MessageModel();
        messageModel.setId(id);
        messageModel.setTimeStamp(timeStamp);
        messageModel.setSpeed(speed);
        messageModel.setDirection(direction);
        messageModel.setLat(lat);
        messageModel.setLon(lon);
        messageModel.setAce(ace);
        messageModel.setMac(mac);
        messageModel.setType(type);
        messageModel.insert();
        sendLog("插入Message成功");
    }

    private void insertResult(int id, int time, double distance, boolean warning, int type, long timeStamp1, long receiverTimeStamp, long sendTimeStamp, long timeStamp2){
        ResultModel resultModel = new ResultModel();
        resultModel.setId(id);
        resultModel.setTime(time);
        resultModel.setDistance(distance);
        resultModel.setWarning(warning);
        resultModel.setType(type);
        resultModel.setTimeStamp1(timeStamp1);
        resultModel.setReceiverTimeStamp(receiverTimeStamp);
        resultModel.setSendTimeStamp(sendTimeStamp);
        resultModel.setTimeStamp2(timeStamp2);
        resultModel.insert();
        sendLog("插入Result成功");
    }

    public void sendLog(String log){
        Intent intent = new Intent();
        intent.setAction(ReceiveThread.ACTION_STRING);//告诉android将要执行什么功能
        intent.putExtra(ReceiveThread.STRING_CONTEXT, log);//信息
        getApplicationContext().sendBroadcast(intent);//广播信息
    }

    public void sendSQL(String log){
        Intent intent = new Intent();
        intent.setAction(MainActivity.ACTION_SQL);//告诉android将要执行什么功能
        intent.putExtra(ReceiveThread.STRING_CONTEXT, log);//信息
        getApplicationContext().sendBroadcast(intent);//广播信息
    }

}
