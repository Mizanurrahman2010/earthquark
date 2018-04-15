<?php
/**
 * Created by PhpStorm.
 * User: Mizan
 * Date: 07-Apr-18
 * Time: 23:28
 */

require 'PHPMailerAutoload.php';
require_once 'CommunicationList.php';

$Name = $RichterScale = $Location = $Time = $WaterLevel = "";

if ($_SERVER["REQUEST_METHOD"] == "POST" AND isset($_GET['l']))
{
    $RichterScale = $_GET["r"];
    $Location     = $_GET["l"];
    $Time         = $_GET["t"];
    $WaterLevel   = $_GET["w"];

    // start: set current time
    date_default_timezone_set('Asia/Dhaka');
    $Time = date("H:i:s");
    // start: set current time

    /* Start : mail */

    $mail = new PHPMailer;

    $mail->SMTPDebug = 0;                               // Enable verbose debug output

    $mail->isSMTP();                                      // Set mailer to use SMTP
    $mail->Host = 'smtp.gmail.com';  // Specify main and backup SMTP servers
    $mail->SMTPAuth = true;                               // Enable SMTP authentication
    $mail->Username = 'earthquake.nsu@gmail.com';                 // SMTP username
    $mail->Password = 'nsu12345';                           // SMTP password
    $mail->SMTPSecure = 'ssl';                            // Enable TLS encryption, `ssl` also accepted
    $mail->Port = 465;                                    // TCP port to connect to

    $mail->setFrom('earthquake.nsu@gmail.com', 'Earthquake');

    foreach ($emails as $email)
    {
        $mail->addAddress($email);     // Add a recipient
    }

    $mail->isHTML(true);                                  // Set email format to HTML

    $mail->Subject = 'Earthquake Alert';

    // $mail->Body .= 'Richter Scale: '.$RichterScale.'<br>';
    $mail->Body .= 'Location: '.$Location.'<br>';
    $mail->Body .= 'Time: '.$Time.'<br>';
    // $mail->Body .= 'WaterLevel: '.$WaterLevel.'<br>';

    // $mail->AltBody .= 'Richter Scale: '.$RichterScale.'<br>';
    $mail->AltBody .= 'Location: '.$Location.'<br>';
    $mail->AltBody .= 'Time: '.$Time.'<br>';
    // $mail->AltBody .= 'Water Level: '.$WaterLevel.'<br>';

    if(!$mail->send())
    {
        echo 0;
        //echo 'Message could not be sent.';
        //echo 'Mailer Error: ' . $mail->ErrorInfo;
    } else {
        echo 1;
        //echo 'Message has been sent';
    }

    /* End : mail */

    /* Start : SMS */
    if ($mobiles)
    {
        //step 2, sendfunction//
        $to = implode(",",$mobiles);
        $token = "14c252082914fcae96324d274d021cc4";
        $message = "Earthquake Alert. Location: $Location, Time: $Time,";

        $url = "http://sms.greenweb.com.bd/api.php";

        $data= array(
            'to'=>"$to",
            'message'=>"$message",
            'token'=>"$token"
        ); // Add parameters in key value
        $ch = curl_init(); // Initialize cURL
        curl_setopt($ch, CURLOPT_URL,$url);
        curl_setopt($ch, CURLOPT_POSTFIELDS, http_build_query($data));
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
        $smsresult = curl_exec($ch);
        //sendsms end//

        if(strlen($smsresult) < 80)
        {
            echo 0; // failed
        }
        else
        {
            echo 1; // success
        }
    }
    /* End : SMS */
}