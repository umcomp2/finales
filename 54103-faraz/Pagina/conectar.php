<?php
session_start();
$usuario_id = $_SESSION['usuario_id'];
if (empty($usuario_id)){
  header('Location:inicio.php');
  exit;
}
$mysqli = new mysqli('192.168.15.203', 'root', 'root', 'computacion2');
$i=0;
$array_pedido = array();
foreach ($_POST['articulos'] as $key => $codigo ) {
    $query = $mysqli -> query ("SELECT codigo,descripcion AS articulo FROM producto WHERE id = $codigo");
    while ($valores = mysqli_fetch_array($query)) {
      if($_POST['cantidad'][$key]!= '0'){
        $array_pedido[$i++] =  $valores['codigo']." ".$valores['articulo']." ".$_POST['cantidad'][$key];
      }
    }
}

$articulos = implode(";",$array_pedido);
if(empty($articulos)){
  header('Location:formulario.php');
  exit;
}else{
  // cliente
  $pedido = "0:/ usuario_id:".$usuario_id." --> ".$articulos.";";
  //echo "envie--->".$pedido;
  $socket = stream_socket_client('tcp://localhost:7000');
  fwrite($socket,$pedido);
  fclose($socket);
  // servidor
  $server2 = stream_socket_server('tcp://localhost:9000');
  $socket2 = stream_socket_accept($server2);
  $archivo_id = stream_socket_recvfrom($socket2, 1500);
  //echo "<br>recibi----> " . $archivo_id . "\n";
  fclose($socket2);
  if (!empty($archivo_id) && $archivo_id !='Error'){
    header('Location:resultado.php?archivo_id='.$archivo_id);
  }
}
?>
