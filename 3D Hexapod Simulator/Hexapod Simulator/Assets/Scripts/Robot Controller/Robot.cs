using System;
using System.Collections;
using System.Threading;
using UnityEngine;

public class Robot : MonoBehaviour
{
    [Header("Robot Config")]
    public ArticulationBody robotBody;
    public Leg fr; //front right leg
    public Leg fl; //front left leg
    public Leg mr; //middle right leg
    public Leg ml; //middle left leg
    public Leg br; //back right leg
    public Leg bl; //back left leg

    public bool inMotion;
    public float multiplier = 0.05f;

    [Header("Debug")]
    public int iters = 90; 
    public float gizmorRad = 0.005f;
    public float dist = 0.05f; //d

    public IEnumerator MoveForward(float distance)
    {
        
        float distInc = distance / iters;
        float Ydist;
        float Zdist; 
        if (!inMotion)
        {
            Vector3 tip = fr.legTip.position;
            inMotion = true;
            for (int i = 1; i <= iters; i++)
            {
                Zdist = distInc * i;
                Ydist = multiplier * Mathf.Sin((i * Mathf.PI / 180) + i * distInc);
                Vector3 tarLoc = tip + new Vector3(0, Ydist, Zdist);
                //Gizmos.DrawSphere(tarLoc, gizmorRad);
                MoveForwardLeg(fr, tarLoc);
                yield return new WaitForSeconds(0.02f);
            }
            inMotion = false;
        }
        // MoveForwardLeg(fr, distance);
        // MoveForwardLeg(br, distance);
        // MoveForwardLeg(fl, distance);
        // MoveForwardLeg(mr, distance);
        // MoveForwardLeg(ml, distance);
        // MoveForwardLeg(bl, distance);
        yield break;
    }

    private void OnDrawGizmos()
    {
        
        float distInc = dist / iters; //e
        float Ydist; //n
        float Zdist; //m
        if (!inMotion)
        {
            Vector3 tip = fr.legTip.position;
            inMotion = true;
            for (int i = 1; i <= iters; i++)
            {
                Zdist = distInc * i;
                Ydist = multiplier * Mathf.Sin((i * Mathf.PI / 180)+i*distInc);
                Vector3 tarLoc = tip + new Vector3(0, Ydist, Zdist);
                Gizmos.DrawSphere(tarLoc, gizmorRad);
                //MoveForwardLeg(fr, tarLoc);
            }
            inMotion = false;
        }
        Gizmos.DrawSphere(fr.legTip.position, gizmorRad);
    }

    public void MoveForwardLeg(Leg leg, Vector3 target)
    {
        Vector3 legTipLoc = leg.legTip.position;
        Vector3 HipPivotLocation = leg.hipRotate.transform.position;
        Vector3 diffVecTarget = target - HipPivotLocation;

        //Debug.DrawLine(legTipLoc, target, Color.green, 600f);
        Debug.DrawLine(HipPivotLocation, target, Color.magenta, 600f);
        //Debug.DrawLine(HipPivotLocation, leg.hipLift.transform.position, Color.cyan, 600f);
        //Debug.DrawLine(leg.hipLift.transform.position, targetLoc, Color.blue, 600f);

        double newHipAngle;
        double newLiftAngle;
        double newKneeAngle;

        //calculate the leg length 
        double len1 = Math.Sqrt(Math.Pow(diffVecTarget.x, 2) + Math.Pow(diffVecTarget.z, 2));
        double len1Real = Vector3.Distance(HipPivotLocation, target);
        double len3 = len1 - leg.coxaOffset;
        double len2 = Math.Sqrt(Math.Pow(len3, 2) + Math.Pow(diffVecTarget.y, 2));
        double len2Real = Vector3.Distance(leg.hipLift.transform.position, target);

        //calculate assistive values for IK when getting the hip rotation angles
        Vector3 diffVecStart = legTipLoc - HipPivotLocation;
        double alpha4 = Math.Atan2(diffVecStart.z, diffVecStart.x) * 180 / Math.PI;
        double alpha5 = 180 - (alpha4 + leg.rotationAngle);


        double alpha6 = Math.Atan2(diffVecTarget.z, diffVecTarget.x) * 180 / Math.PI; //value in degrees


        //calculate assistive values for IK when getting values for knee and lift angles
        //double alpha1 = Math.Atan2(len3, diffVecTarget.y) * 180 / Math.PI; //value in degrees
        double alpha1 = Math.Acos(Math.Abs(diffVecTarget.y) / len2) * 180 / Math.PI; //value in degrees

        double alpha2 = Math.Acos(
            (Math.Pow(leg.tibiaLength, 2) - Math.Pow(leg.femurLength, 2) - Math.Pow(len2, 2)) /
            (-2 * leg.femurLength * len2)
        ) * 180 / Math.PI; //value in degrees

        double alpha3 = Math.Acos(
            (Math.Pow(len2, 2) - (Math.Pow(leg.femurLength, 2)) - Math.Pow(leg.tibiaLength, 2)) /
                (-2 * leg.femurLength * leg.tibiaLength)
            ) * 180 / Math.PI;//value in degrees

        newLiftAngle = alpha1 + alpha2;

        newKneeAngle = 180 - alpha3;

        if (leg.isLeft)
        {
            //calculate the hip rotation angle
            //  same as: 90 - Atan(Xd/Zd)
            newHipAngle = 180 - (alpha6 + alpha5);
        }
        else
        {
            //calculate the hip rotation angle
            //  same as: 90 - Atan(Xd/Zd)
            newHipAngle = 180 - (alpha6 + alpha5);
        }

        leg.moveLeg(newHipAngle, newLiftAngle, newKneeAngle);

        Debug.Log("Leg: " + leg.name +
            "\nNew Hip Rotation angle: " + newHipAngle +
                  "\nNew Hip Lift angle: " + newLiftAngle +
                  "\nNew Knee bend angle: " + newKneeAngle +
                  "\nalpha1: " + alpha1 +
                  "\nalpha2: " + alpha2 +
                  "\nalpha3: " + alpha3 +
                  "\nCurrent Vector Loc: " + legTipLoc.ToString("F5") +
                  "\nDifference Vector: " + diffVecTarget.ToString("F5") +
                  "\nTarget Vector Loc: " + target.ToString("F5") +
                  "\nLen 1: " + len1 +
                  "\nLen 2 (smart): " + Vector3.Distance(leg.hipLift.transform.position, target) +
                  "\nLen 2: " + len2);

        //Debug.DrawLine(HipPivotLocation, leg.legTip.position, Color.cyan, 600f);
    }


    public void MoveForwardLeg(Leg leg, float distance)
    {
        Vector3 legTipLoc = leg.legTip.position;
        Vector3 targetLoc = legTipLoc + new Vector3(0f, 0f, distance);
        Vector3 HipPivotLocation = leg.hipRotate.transform.position;
        Vector3 diffVecTarget = targetLoc - HipPivotLocation;

        //Debug.DrawLine(legTipLoc, targetLoc, Color.green, 600f);
        Debug.DrawLine(HipPivotLocation, targetLoc, Color.magenta, 600f);
        //Debug.DrawLine(HipPivotLocation, leg.hipLift.transform.position, Color.cyan, 600f);
        //Debug.DrawLine(leg.hipLift.transform.position, targetLoc, Color.blue, 600f);

        double newHipAngle;
        double newLiftAngle;
        double newKneeAngle;

        //calculate the leg length 
        double len1 = Math.Sqrt(Math.Pow(diffVecTarget.x, 2) + Math.Pow(diffVecTarget.z , 2));
        double len1Real = Vector3.Distance(HipPivotLocation, targetLoc);
        double len3 = len1 - leg.coxaOffset; 
        double len2 = Math.Sqrt(Math.Pow(len3, 2) + Math.Pow(diffVecTarget.y, 2));
        double len2Real = Vector3.Distance(leg.hipLift.transform.position, targetLoc);

        //calculate assistive values for IK when getting the hip rotation angles
        Vector3 diffVecStart = legTipLoc - HipPivotLocation;
        double alpha4 = Math.Atan2(diffVecStart.z, diffVecStart.x) * 180 / Math.PI;
        double alpha5 = 180 - (alpha4 + leg.rotationAngle);


        double alpha6 = Math.Atan2(diffVecTarget.z, diffVecTarget.x) * 180 / Math.PI; //value in degrees


        //calculate assistive values for IK when getting values for knee and lift angles
      //double alpha1 = Math.Atan2(len3, diffVecTarget.y) * 180 / Math.PI; //value in degrees
        double alpha1 = Math.Acos(Math.Abs(diffVecTarget.y)/len2) * 180 / Math.PI; //value in degrees

        double alpha2 = Math.Acos(
            (Math.Pow(leg.tibiaLength, 2) - Math.Pow(leg.femurLength, 2) - Math.Pow(len2, 2)) /
            (-2 * leg.femurLength * len2)
        ) * 180 / Math.PI; //value in degrees

        double alpha3 = Math.Acos(
            (Math.Pow(len2, 2) - (Math.Pow(leg.femurLength, 2)) - Math.Pow(leg.tibiaLength, 2)) / 
                (-2 * leg.femurLength * leg.tibiaLength)
            ) * 180 / Math.PI;//value in degrees
        
        newLiftAngle = alpha1 + alpha2;

        newKneeAngle = 180 - alpha3;

        if (leg.isLeft)
        {
            //calculate the hip rotation angle
            //  same as: 90 - Atan(Xd/Zd)
            newHipAngle = 180 - (alpha6 + alpha5);
        }
        else
        {
            //calculate the hip rotation angle
            //  same as: 90 - Atan(Xd/Zd)
            newHipAngle = 180 - (alpha6 + alpha5);
        }

        leg.moveLeg(newHipAngle, newLiftAngle, newKneeAngle);
        
        Debug.Log( "Leg: " + leg.name +
            "\nNew Hip Rotation angle: " + newHipAngle +
                  "\nNew Hip Lift angle: " + newLiftAngle +
                  "\nNew Knee bend angle: " + newKneeAngle +
                  "\nalpha1: " + alpha1 +
                  "\nalpha2: " + alpha2 +
                  "\nalpha3: " + alpha3 +
                  "\nCurrent Vector Loc: " + legTipLoc.ToString("F5") +
                  "\nDifference Vector: " + diffVecTarget.ToString("F5") +
                  "\nTarget Vector Loc: " + targetLoc.ToString("F5") +
                  "\nLen 1: " + len1 +
                  "\nLen 2 (smart): " + Vector3.Distance(leg.hipLift.transform.position, targetLoc) +
                  "\nLen 2: " + len2);
        
        Debug.DrawLine(HipPivotLocation, leg.legTip.position, Color.cyan, 600f);
    }

    public void MoveBackwards(float distance)
    {
        StartCoroutine(MoveForward(-distance));
    }

    public void StrafeRight(float distance)
    {

    }
    public void StafeLeft(float distance)
    {
        StrafeRight(-distance);
    }

}
