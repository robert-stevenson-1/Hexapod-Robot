using UnityEngine;
using UnityEngine.UI;

public class UI : MonoBehaviour
{
    public RobotController robotController;

    public Button btnDisconnect;
    void Start()
    {
        btnDisconnect.onClick.AddListener(disconnectRobot);
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    void disconnectRobot()
    {
        robotController.disconnectCOM();
    }
}
