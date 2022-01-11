using System;
using System.Collections;
using UnityEngine;
using System.IO.Ports;


public class RobotController : MonoBehaviour
{
    [Header("Robot Assignment")]
    public Robot robot;

    [Header("Robot Communication")] 
    public String comPort = "COM4";
    public int baudRate = 9600;
    public int readTimeout = 200;
    public int writeTimeout = 100;
    public SerialPort arduinoStream;
    [Space(5)]
    public float nextTime;
    public float sampleRate;
    [Space(5)]
    public string data = "";
    public string lastData = "";
    [Space(5)]
    public bool canReceive = true;
    public bool canTransmit = true;

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
            arduinoStream = new SerialPort(comPort, baudRate);
            arduinoStream.ReadTimeout = 50;
            arduinoStream.ReadBufferSize = 8192;
            arduinoStream.WriteBufferSize = 256;
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
        if (arduinoStream.IsOpen)
        {
            //check to see if we are allowed to send data to the robot
            if (canTransmit)
            {
                string toSend = getRobotData(robot);
                sendRobot(toSend, writeTimeout);
                canTransmit = false;
                canReceive = true;
            }

            //check to see if we are allowed to read data from the robot
            if (canReceive)
            {
                StartCoroutine(ReadRobot(200f));
                canTransmit = true;
                canReceive = false;
            }
        }
        else
        {
            Debug.LogWarning("Port Not Open");
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

    // string ReadRobot(int timeout = 1)
    // {
    //     //arduinoStream.ReadTimeout = timeout;
    //     try
    //     {
    //         string value = arduinoStream.ReadLine();
    //         return value;
    //     }
    //     catch (System.TimeoutException e)
    //     {
    //         print(e);
    //         return null;
    //     }
    // }
    public IEnumerator ReadRobot(float timeout = 10000f)
    {
        DateTime initialTime = DateTime.Now;
        DateTime nowTime;
        TimeSpan diff = default(TimeSpan);
        string value;
        //arduinoStream.ReadTimeout = timeout;
        do
        {
            try
            {
                value = arduinoStream.ReadLine();
            }
            catch (TimeoutException e)
            {
                value = "";
                print(e);
            }

            if (value != "")
            {
                data = value;
                yield break;
            }

            nowTime = DateTime.Now;
            diff = nowTime - initialTime;
        } while (diff.Milliseconds < timeout);
    }

    void sendRobot(string data, int timeout = 1)
    {
        //arduinoStream.WriteTimeout = timeout;
        try
        {
            Debug.Log("Sent: " + data);
            arduinoStream.WriteLine(data);
            arduinoStream.BaseStream.Flush();
        }
        catch (System.TimeoutException e)
        {
            print(e);
        }
    }

    string getRobotData(Robot robot)
    {
        string data = "";

        data += " " + robot.fr.rotationAngle;
        data += " " + robot.fr.liftAngle;
        data += " " + robot.fr.kneeAngle;

        data += " " + robot.mr.rotationAngle;
        data += " " + robot.mr.liftAngle;
        data += " " + robot.mr.kneeAngle;

        data += " " + robot.br.rotationAngle;
        data += " " + robot.br.liftAngle;
        data += " " + robot.br.kneeAngle;

        data += " " + robot.fr.rotationAngle;
        data += " " + robot.fr.liftAngle;
        data += " " + robot.fr.kneeAngle;

        data += " " + robot.mr.rotationAngle;
        data += " " + robot.mr.liftAngle;
        data += " " + robot.mr.kneeAngle;

        data += " " + robot.br.rotationAngle;
        data += " " + robot.br.liftAngle;
        data += " " + robot.br.kneeAngle;

        return data;
    }

    public void disconnectCOM()
    {
        arduinoStream.Close();
    }
}