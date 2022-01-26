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
            Debug.Log("Robot Moving Forward");
            StartCoroutine(robot.MoveForward(movementDistance));
        }

        if (Input.GetKeyDown(KeyCode.S))
        {
            Debug.Log("Robot Moving Backwards");
            robot.MoveBackwards(movementDistance);
        }

        if (Input.GetKeyDown(KeyCode.A)) //TODO: right strafe
        {
            Debug.Log("Robot Strafing Left");
            //robot.StafeLeft(movementDistance);
        }

        if (Input.GetKeyDown(KeyCode.D)) //TODO: right strafe
        {
            Debug.Log("Robot Strafing Right");
            //robot.StrafeRight(movementDistance);
        }

    }
}
