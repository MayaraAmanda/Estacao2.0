<?php
	try{
		$HOST = "localhost";
		$DB = "estacao";
		$USUARIO = "usuario";
		$SENHA = "123";

		$PDO = new PDO("mysql:host=". $HOST . ";dbname=". $DB . ";charset=utf8", $USUARIO, $SENHA);
	}catch(PDOException $erro){
		//echo "Erro de conexao, detalhes: " . $erro->getMessage();
		echo "erro de conexao";
	}

?>