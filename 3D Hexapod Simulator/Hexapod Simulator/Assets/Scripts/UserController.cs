using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UserController : MonoBehaviour
{
    public Robot robot;
    public float movementDistance;

    // Start is called before the first frame update
    void Start()
    {
        if (robot == null)
        {
            Debug.LogError("robot to control not set");
            return;
        }
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.W))
        {
            Debug.Log("Robot Moving Foward");
            robot.MoveForward(movementDistance);
        }

        if (Input.GetKeyDown(KeyCode.S))
        {
            Debug.Log("Robot Moving Backwards");
            robot.MoveBackwards(movementDistance);
        }

        if (Input.GetKeyDown(KeyCode.A))
        {
            Debug.Log("Robot Stafing Left");
            robot.StafeLeft(movementDistance);
        }

        if (Input.GetKeyDown(KeyCode.D))
        {
            Debug.Log("Robot Stafing Right");
            robot.StrafeRight(movementDistance);
        }

    }
}
