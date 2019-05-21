<?php
session_start();
$mysqli = new mysqli('192.168.15.203', 'root', 'root', 'computacion2');

$usuario = $_POST['usuario'];
$pass = $_POST['pass'];


$query = $mysqli -> query ("SELECT * FROM usuario WHERE nickname = '$usuario'");

$usuario_db = mysqli_fetch_array($query);

if ($usuario_db['pass'] === $pass){
  $_SESSION['usuario_id'] = $usuario_db['id'];
  header('Location: formulario.php');
exit;
}else {
  header('Location: inicio.php');
exit;
}

?>
