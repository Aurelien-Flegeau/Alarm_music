<?php 
	require('phpmailer/class.phpmailer.php');
	$mail = new PHPMailer();
	$mail->Host = 'smtp.domaine.fr';
	$mail->SMTPAuth   = false;
	$mail->Port = 25; // Par défaut
	 
	// Expéditeur
	$mail->SetFrom('expediteur@teub.com', 'Nom Prénom');
	// Destinataire
	$mail->AddAddress('kev.dhorne@gmail.com', 'Nom Prénom');
	// Objet
	$mail->Subject = 'Objet du message';
	 
	// Votre message
	$mail->MsgHTML('ALARME ALARME ALARME <3<3<3 !');
	 
	// Envoi du mail avec gestion des erreurs
	if(!$mail->Send()) {
	  echo 'Erreur : ' . $mail->ErrorInfo;
	} else {
	  echo 'Message envoyé !';
	} 
?>

