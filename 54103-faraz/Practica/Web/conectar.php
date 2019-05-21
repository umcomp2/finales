<?php
session_start();

$usuario_id = $_SESSION['usuario_id'];
$mysqli = new mysqli('localhost', 'nfaraz', '1424/4', 'computacion2');
$socket = stream_socket_client('tcp://localhost:7700');

$i=0;
$array_pedido = array();
foreach ($_POST['articulos'] as $key => $codigo ) {
    $query = $mysqli -> query ("SELECT descripcion AS articulo FROM producto WHERE id = $codigo");
    while ($valores = mysqli_fetch_array($query)) {
      $array_pedido[$i++] =  $codigo." ".$valores['articulo']." ".$_POST['cantidad'][$key];
    }
}

$articulos = implode(";",$array_pedido);
$pedido = "usuario_id: ".$usuario_id."->".$articulos;
echo $pedido;
fwrite($socket,$pedido);
fclose($socket);


/* Abrir un socket al puerto 1234 en localhost */
//$socket = stream_socket_client('tcp://localhost:7700');

/* Enviar información ordinaria mediante canales oridinarios. */
//fwrite($socket,$_POST['codigo']);

/* Enviar más información fuera de banda. */
//stream_socket_sendto($socket, "Información Fuera de Banda.", STREAM_OOB);

/* Cerrarlo */
//fclose($socket);

?>
