using UnityEngine;

public class CameraFollow : MonoBehaviour
{
    public GameObject target;        //Public variable to store a reference to the target you want to follow with the camera

    public Vector3 offset;            //public variable to store the offset distance between the target and camera

    public bool lookAtTarget = true;
    public bool lookAroundTarget = true;
    [Range(0.1f, 5f)]
    public float rotationSpeed = 2f;
    [Range(0.01f, 1.0f)]
    public float smoothFactor = 0.1f;

    // Use this for initialization
    void Start()
    {
        //Calculate and store the offset value by getting the distance between the player's position and camera's position.
        offset = transform.position - target.transform.position;
    }

    
    // LateUpdate is called after Update each frame
    void LateUpdate()
    {
        //if right clicking with mouse and dragging the cursor then rotate around the targets left and right
        if (Input.GetMouseButton(1))
        {
            // Set the position of the camera's transform to be the same as the player's, but offset by the calculated offset distance.
            if (lookAroundTarget)
            {
                Quaternion camTurnAngleX = Quaternion.AngleAxis(Input.GetAxis("Mouse X") * rotationSpeed, Vector3.up);
                offset = camTurnAngleX * offset;
            }
        }

        //if left clicking with mouse and dragging the cursor then rotate around the targets up or down
        if (Input.GetMouseButton(0))
        {
            if (lookAroundTarget)
            {
                Quaternion camTurnAngleY = Quaternion.AngleAxis(Input.GetAxis("Mouse Y") * rotationSpeed, Vector3.right);
                offset = camTurnAngleY * offset;
            }
        }

        if (Input.GetAxis("Mouse ScrollWheel") > 0)
        {
            if (Camera.main.fieldOfView > 1)
            {
                Camera.main.fieldOfView--;
            }
        }

        if (Input.GetAxis("Mouse ScrollWheel") < 0)
        {
            if (Camera.main.fieldOfView < 100)
            {
                Camera.main.fieldOfView++;
            }
        }

        Vector3 newPos = target.transform.position + offset;

        transform.position = Vector3.Slerp(transform.position, newPos, smoothFactor);

        if (lookAtTarget || lookAroundTarget)
        {
            transform.LookAt(target.transform);
        }
    }
}
