package org.jinkia.jk_rockerview;

import android.bluetooth.BluetoothAdapter;
import android.content.Intent;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.SeekBar;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;

import java.util.HashMap;

import static org.jinkia.jk_rockerview.RockerView.DirectionMode.DIRECTION_8;


public class MainActivity extends AppCompatActivity {

    private RockerView mRockerView;
    private TextView mTvShake;
    private TextView mTvAngle;
    private TextView mTvLevel;
    private TextView mTvModel;

    private Switch keepLine;
    private Switch goAvoid;

    //功能控件
    private Button btnPairedDevices;
    private SeekBar speedBar;
    private TextView speed_val;
    private TextView speed_now;
    private static final int REQUEST_ENABLE_BT = 1;
    private int speed = 70;


    //蓝牙相关
    private BluetoothAdapter mBluetoothAdapter;
    private ConnectedThread mConnectedThread;

    HashMap<String, Integer> btn_actions = new HashMap<String, Integer>();


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mRockerView = (RockerView) findViewById(R.id.my_rocker);
        mTvAngle = (TextView) findViewById(R.id.tv_now_angle);
        mTvLevel = (TextView) findViewById(R.id.tv_now_level);
        mTvModel = (TextView) findViewById(R.id.tv_now_model);
        mTvShake = (TextView) findViewById(R.id.tv_now_shake);

        speedBar = (SeekBar) findViewById(R.id.speedBar);
        keepLine = (Switch) findViewById(R.id.keepLine);
        goAvoid = (Switch) findViewById(R.id.goAvoid);
        speed_val = (TextView) findViewById(R.id.speed_val);
        speed_now= (TextView) findViewById(R.id.speed_now);

        btn_actions.put("goHead", 2);//前
        btn_actions.put("goBack", 8);//后
        btn_actions.put("goLeft", 4);//左
        btn_actions.put("goRight", 6);//右
        btn_actions.put("goHeadLeft", 1);//左前
        btn_actions.put("goHeadRight", 3);//右前
        btn_actions.put("goBackLeft", 7);//左后
        btn_actions.put("goBackRight", 9);//右后

        btn_actions.put("goCircle", 12);//打转
        btn_actions.put("doStop", 5);//停车

        btn_actions.put("goOnline", 10);//寻线
        btn_actions.put("goAvoid", 11);//避障
        btn_actions.put("keepAhead", 1003);//


        //蓝牙扩展
        btnPairedDevices = (Button) findViewById(R.id.btnPairedDevices);


        mTvModel.setText("当前模式：方向有改变时回调；8个方向");
        speedBar.setProgress(speed);
        speed_val.setText(String.valueOf(speed));
        speedBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean b) {
                speed = progress;
                speed_val.setText(String.valueOf(speed));

            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                sendString(String.valueOf(speed));
                speed_now.setText("当前速度："+String.valueOf(speed));
            }
        });

        keepLine.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (!keepLine.isChecked()) {
//                    keepLine.setChecked(false);
                    sendString(String.valueOf(13));
                } else {
//                    keepLine.setChecked(true);
                    goAvoid.setChecked(false);
                    sendString(String.valueOf(btn_actions.get("goOnline")));
                    speed=80;
                    speedBar.setProgress(speed);
                    speed_now.setText("当前速度："+String.valueOf(speed));
                }
            }
        });

        goAvoid.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (!goAvoid.isChecked()) {
//                    goAvoid.setChecked(false);
                    sendString(String.valueOf(13));
                } else {
//                    goAvoid.setChecked(true);
                    keepLine.setChecked(false);
                    sendString(String.valueOf(btn_actions.get("goAvoid")));
                    speed=80;
                    speedBar.setProgress(speed);
                    speed_now.setText("当前速度："+String.valueOf(speed));
                }
            }
        });

        mRockerView.setOnShakeListener(DIRECTION_8, new RockerView.OnShakeListener() {
            @Override
            public void onStart() {

            }

            @Override
            public void direction(RockerView.Direction direction) {
                if (direction == RockerView.Direction.DIRECTION_CENTER) {
                    sendString(String.valueOf(btn_actions.get("doStop")));
                    mTvShake.setText("停车");
                } else if (direction == RockerView.Direction.DIRECTION_DOWN) {
                    sendString(String.valueOf(btn_actions.get("goBack")));
                    mTvShake.setText("倒车");
                } else if (direction == RockerView.Direction.DIRECTION_LEFT) {
                    sendString(String.valueOf(btn_actions.get("goLeft")));
                    mTvShake.setText("左转弯");
                } else if (direction == RockerView.Direction.DIRECTION_UP) {
                    sendString(String.valueOf(btn_actions.get("goHead")));
                    mTvShake.setText("前进");
                } else if (direction == RockerView.Direction.DIRECTION_RIGHT) {
                    sendString(String.valueOf(btn_actions.get("goRight")));
                    mTvShake.setText("右转弯");
                } else if (direction == RockerView.Direction.DIRECTION_DOWN_LEFT) {
                    sendString(String.valueOf(btn_actions.get("goBackLeft")));
                    mTvShake.setText("左后转");
                } else if (direction == RockerView.Direction.DIRECTION_DOWN_RIGHT) {
                    sendString(String.valueOf(btn_actions.get("goBackRight")));
                    mTvShake.setText("右后转");
                } else if (direction == RockerView.Direction.DIRECTION_UP_LEFT) {
                    sendString(String.valueOf(btn_actions.get("goHeadLeft")));
                    mTvShake.setText("左前转");
                } else if (direction == RockerView.Direction.DIRECTION_UP_RIGHT) {
                    sendString(String.valueOf(btn_actions.get("goHeadRight")));
                    mTvShake.setText("右前转");
                }
            }

            @Override
            public void onFinish() {

            }
        });
        mRockerView.setOnAngleChangeListener(new RockerView.OnAngleChangeListener() {
            @Override
            public void onStart() {

            }

            @Override
            public void angle(double angle) {
                mTvAngle.setText("当前角度：" + angle);
            }

            @Override
            public void onFinish() {

            }
        });

        mRockerView.setOnDistanceLevelListener(new RockerView.OnDistanceLevelListener() {
            @Override
            public void onDistanceLevel(int level) {
                mTvLevel.setText("当前距离级别：" + level);
            }
        });


        btnPairedDevices.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                // 获取蓝牙适配器
                mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
                if (mBluetoothAdapter == null) {
                    Toast.makeText(getApplicationContext(), "该设备不支持蓝牙", Toast.LENGTH_SHORT).show();
                }

                //请求开启蓝牙
                if (!mBluetoothAdapter.isEnabled()) {
                    Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                    startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
                }

                //进入蓝牙设备连接界面
                Intent intent = new Intent();
                intent.setClass(getApplicationContext(), DevicesListActivity.class);
                startActivity(intent);

            }
        });
    }

    @Override
    protected void onResume() {
        super.onResume();


        //回到主界面后检查是否已成功连接蓝牙设备
        if (BluetoothUtils.getBluetoothSocket() == null || mConnectedThread != null) {
            Toast.makeText(getApplicationContext(), "未连接",
                    Toast.LENGTH_SHORT).show();
            return;
        }

        Toast.makeText(getApplicationContext(), "已连接",
                Toast.LENGTH_SHORT).show();

        Handler handler = new Handler() {
            @Override
            public void handleMessage(Message msg) {
                super.handleMessage(msg);
                switch (msg.what) {
                    case ConnectedThread.MESSAGE_READ:
                        byte[] buffer = (byte[]) msg.obj;
                        int length = msg.arg1;
                        for (int i = 0; i < length; i++) {
                            char c = (char) buffer[i];
//                            edtReceivedMessage.getText().append(c);
                        }
                        break;
                }

            }
        };

        mConnectedThread = new ConnectedThread(BluetoothUtils.getBluetoothSocket(), handler);
        mConnectedThread.start();
    }


    public void sendString(String temp) {
        if (temp.isEmpty()) {
            return;
        }
        String sendStr = temp;
        char[] chars = sendStr.toCharArray();
        byte[] bytes = new byte[chars.length];
        for (int i = 0; i < chars.length; i++) {
            bytes[i] = (byte) chars[i];
        }
        try {
            mConnectedThread.write(bytes);
        } catch (Exception e) {
            Intent intent = new Intent();
            intent.setClass(getApplicationContext(), DevicesListActivity.class);
            startActivity(intent);
            Toast.makeText(getApplicationContext(), "未连接",
                    Toast.LENGTH_SHORT).show();
        }
    }
}
