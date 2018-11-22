<!doctype html>
<html>
<head>
<title></title>
<meta charset="UTF-8">
<script
  src="https://code.jquery.com/jquery-3.3.1.min.js"
  integrity="sha256-FgpCb/KJQlLNfOu91ta32o/NMZxltwRo8QtmkMRdAu8="
  crossorigin="anonymous"></script>
<!-- Boostrap JavaScript -->
    

	

<link rel="stylesheet" type="text/css" href="css/style.css"> 
<link rel="stylesheet" type="text/css" href="css/forms.css">
<link rel="stylesheet" type="text/css" href="css/bootstrap.css">
</head>
<body>
<div class="container">
<form id="form" method="POST">
<div class="form-group">
<select class="form-control" id="method" name="method">
				<option>Metot Seçiniz...</option>
					  <!--<option value="1">Hızlı Metot</option>
				  <option value="2">Standart Metot</option>-->
				<option value="3">Otomatik Hazırlama</option>
				<option value="4">Manuel Hazırlama</option>
</select>
<p><?php if (isset($hata['metotsecimhata1'])) echo $hata['metotsecimhata1']; ?></p>
<div class="form-error"></div>
<br>
</div>
<div class="form-group">
<input class="form-control" id="name" type="text" name="name" placeholder="Ad Soyad" >
<p><?php if (isset($hata['adHata1'])) echo $hata['adHata1']; ?></p>
<p><?php if (isset($hata['adHata2'])) echo $hata['adHata2']; ?></p>
<div class="form-error"></div>
<br>
</div>
<div class="form-group">
<input class="form-control" id="email" type="text" name="email" placeholder="E-Posta">
<p><?php if (isset($hata['emailHata1'])) echo $hata['emailHata1']; ?></p>
<p><?php if (isset($hata['emailHata2'])) echo $hata['emailHata2']; ?></p>
<p><?php if (isset($hata['emailHata3'])) echo $hata['emailHata3']; ?></p>
<div class="form-error"></div>
<br>
</div>
<div class="form-group">
<textarea class="form-control" id="mesaj" name="mesaj" style="height: 154px;" placeholder="Mesajınız"></textarea>
<div class="form-error"></div>
<br>
</div>
<div class="form-group">
<button class="btn btn-primary" id="submit" type="submit" name="submit">Metodu Göster</button>
<p class="form-message"></p>
</div>
</form>
</div>
<script src="js/jqueryvalidate.js"></script>
<script>
$("form").jqueryValidate({
	url: "server.php",
	validCallback: function(){
		alert("Form is valid");
	},
	errorContainer:'.form-error'
});

</script>
</body>
</html>
