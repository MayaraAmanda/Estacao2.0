<?php 
	
	include('conexao.php');
	
	$anemometro = $_GET['anemometro'];
	$barometro = $_GET['barometro'];
	$higrometro = $_GET['higrometro'];
	$piranometro = $_GET['piranometro'];
	$pluviografo = $_GET['pluviografo'];
	$termometro = $_GET['termometro'];
	

	$sql = "INSERT INTO dados (anemometro, barometro, higrometro, piranometro, pluviografo, termometro) VALUES (:anemometro, :barometro, :higrometro, :piranometro, :pluviografo, :termometro)";

	$stmt = $PDO->prepare($sql);

	$stmt->bindParam(':anemometro', $anemometro); //passa os dados para cada sensor
	$stmt->bindParam(':barometro', $barometro); //passa os dados para cada sensor
	$stmt->bindParam(':higrometro', $higrometro); //passa os dados para cada sensor
	$stmt->bindParam(':piranometro', $piranometro); //passa os dados para cada sensor
	$stmt->bindParam(':pluviografo', $pluviografo); //passa os dados para cada sensor
	$stmt->bindParam(':termometro', $termometro); //passa os dados para cada sensor


	if($stmt->execute()){
		echo "dados salvos com sucesso";
	}else{
		echo "erro ao gravar";
	}
 ?>