<!DOCTYPE html>
<html lang="en" dir="ltr">
<?php
  $mysqli = new mysqli('localhost', 'nfaraz', '1424/4', 'computacion2');
?>
  <head>
    <meta charset="utf-8">
    <title></title>
  </head>
  <body>
    <form action="conectar.php" method="post">
<?php for ($i=0; $i < 6; $i++) {?>

      <select class="" name="articulos[]">
        <?php
          $query = $mysqli -> query ("SELECT id,CONCAT(codigo,' - ',descripcion) AS articulo FROM producto");
          while ($valores = mysqli_fetch_array($query)) {
            echo '<option value="'.$valores['id'].'">'.$valores['articulo'].'</option>';
          }
        ?>
      </select>
      <input type="number" name="cantidad[]" value="">
            <br>
    <?php } ?>
      <br>
      <input type="submit" />
    </form>
  </body>
</html>
<!-- agregar javascript para poder agregar dinamicamente mas intem al pedido
