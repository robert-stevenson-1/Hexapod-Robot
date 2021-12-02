using System;
using UnityEngine;
using System.IO.Ports;

public class RobotController : MonoBehaviour
{
    [Header("Robot Assignment")]
    public Robot robot;
    [Header("Robot Communication Setup")]
    public SerialPort arduinoStream;
    public float nextTime;
    public float sampleRate;
    [Space(5)]
    public string data = "";
    public string lastData = "";

    [Header("Debug")]
    public bool startUpDone;
    public float upTime;
    public bool lockLocation = false;

    // Start is called before the first frame update
    void Start()
    {
        try
        {
            //setup the arduino com port
            arduinoStream = new SerialPort("COM4", 9600);
            arduinoStream.ReadTimeout = 50;
            arduinoStream.Open(); //open the serial com port
            nextTime = Time.time;
        }
        catch (Exception e)
        {
            Debug.LogError(e);
        }


        robot.fr.moveLeg(RobotConfig.FR_ROTATE_INIT_ANGLE, RobotConfig.FR_LIFT_INIT_ANGLE,
            RobotConfig.FR_KNEE_INIT_ANGLE);
        robot.mr.moveLeg(RobotConfig.MR_ROTATE_INIT_ANGLE, RobotConfig.MR_LIFT_INIT_ANGLE,
            RobotConfig.MR_KNEE_INIT_ANGLE);
        robot.br.moveLeg(RobotConfig.BR_ROTATE_INIT_ANGLE, RobotConfig.BR_LIFT_INIT_ANGLE,
            RobotConfig.BR_KNEE_INIT_ANGLE);
        robot.fl.moveLeg(RobotConfig.FL_ROTATE_INIT_ANGLE, RobotConfig.FL_LIFT_INIT_ANGLE,
            RobotConfig.FL_KNEE_INIT_ANGLE);
        robot.ml.moveLeg(RobotConfig.ML_ROTATE_INIT_ANGLE, RobotConfig.ML_LIFT_INIT_ANGLE,
            RobotConfig.ML_KNEE_INIT_ANGLE);
        robot.bl.moveLeg(RobotConfig.BL_ROTATE_INIT_ANGLE, RobotConfig.BL_LIFT_INIT_ANGLE,
            RobotConfig.BL_KNEE_INIT_ANGLE);


        //counter initial startup launch force from intersecting parts on start up
        startUpDone = true;
    }

    // Update is called once per frame
    void Update()
    {
        if (Time.time > nextTime)
        {
            if (arduinoStream.IsOpen)
            {
                data = ReadRobot(100);
            }
            else
            {
                Debug.LogWarning("Port Not Open");
            }

            nextTime = Time.time + sampleRate;
        }

        if (data != null && data != lastData)
        {
            print("Data: " + data);
            lastData = data;
        }
    }

    private void LateUpdate()
    {
        upTime += Time.fixedDeltaTime;
        //counter inital startup launch force from intersecting parts on start up
        if (startUpDone && upTime > 1f && !lockLocation)
        {
            //Unity simulation on only requirement
            robot.robotBody.immovable = false;
            robot.robotBody.velocity = new Vector3(0, 0, 0);
            startUpDone = false;
        }
    }

    string ReadRobot(int timeout = 0)
    {
        arduinoStream.ReadTimeout = timeout;
        try
        {
            return arduinoStream.ReadLine();
        }
        catch (System.TimeoutException e)
        {
            print(e);
            return null;
        }
    }
}