<?php
session_start();
$mysqli = new mysqli('localhost', 'nfaraz', '1424/4', 'computacion2');

$usuario = $_POST['usuario'];
$pass = $_POST['pass'];


$query = $mysqli -> query ("SELECT * FROM usuario WHERE nickname = '$usuario'");

$usuario_db = mysqli_fetch_array($query);

if ($usuario_db['pass'] === $pass){
  $_SESSION['usuario_id'] = $usuario_db['id'];
  header('Location: formulario.php');
exit;
}

?>
