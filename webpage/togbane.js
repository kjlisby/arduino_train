var Train1Position = "B7_loco";
var Train2Position = "B2_loco";

function GetStatus(){
	var request = new XMLHttpRequest();
	request.onreadystatechange = function(){
	if(this.readyState == 4 && this.status == 200) {
		  UpdatePage(this.responseText);
		}
	}
	request.open("GET", "ajax_getstatus&random="+Math.random()*1000000,true);
	request.send(null)
	setTimeout('GetStatus()',10000);
}

function flip(idToFlip, wanted_rotation) {
	document.getElementById(idToFlip).style.transform = "rotate("+wanted_rotation+"deg)";
}

function warm(PS_class) {
  var x = document.getElementsByClassName(PS_class);
  var i;
  for (i = 0; i < x.length; i++) {
    x[i].src = "skinne_varm.gif";
  }
}

function cold(PS_class) {
  var x = document.getElementsByClassName(PS_class);
  var i;
  for (i = 0; i < x.length; i++) {
    x[i].src = "skinne_kold.jpg";
  }
}

function setPower(psu, value) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      UpdatePage(this.responseText);
    }
  };
  xhttp.open("GET", "ajax_set_"+psu+"_"+value, true);
  xhttp.send();
}
var psu1_slider = document.getElementById("Power1");
psu1_slider.oninput = function() {
  setPower("psu1", this.value);
}
var psu2_slider = document.getElementById("Power2");
psu2_slider.oninput = function() {
  setPower("psu2", this.value);
}

var x = document.getElementsByClassName("klikbar");
var i;
for (i = 0; i < x.length; i++) {
  x[i].onclick = function() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        UpdatePage(this.responseText);
      }
  };
  xhttp.open("GET", "ajax_set_"+this.id, true);
  xhttp.send();
  }
}

var output = document.getElementById("demo");
function UpdatePage(jsonString) {
  output.innerHTML = "";
  //output.innerHTML = jsonString;
  var i;
  var obj_list = JSON.parse(jsonString);
  for (i in obj_list.objects) {
    //output.innerHTML += " "+obj_list.objects[i].item+":"+obj_list.objects[i].value;
    if (obj_list.objects[i].item == "automode") {
      var autobox       = document.getElementById("automode");
      var automodelabel = document.getElementById("automodelabel");
      if (obj_list.objects[i].value == "1") {
        automodelabel.style.color = "red";
        autobox.checked = true;
      } else {
        automodelabel.style.color = "black";
        autobox.checked = false;
      }
    } else if (obj_list.objects[i].item == "PSU1FB") {
      output.innerHTML += " "+obj_list.objects[i].item+":"+obj_list.objects[i].value;
    } else if (obj_list.objects[i].item == "PSU2FB") {
      output.innerHTML += " "+obj_list.objects[i].item+":"+obj_list.objects[i].value;
    } else if (obj_list.objects[i].item == "TD1") {
      output.innerHTML += " "+obj_list.objects[i].item+":"+obj_list.objects[i].value;
    } else if (obj_list.objects[i].item == "TD2") {
      output.innerHTML += " "+obj_list.objects[i].item+":"+obj_list.objects[i].value;
    } else if (obj_list.objects[i].item == "Power1") {
      psu1_slider.value = obj_list.objects[i].value;
      if (obj_list.objects[i].value >= 0) {
        flip("B4", 0);
      } else {
        flip("B4", 180);
      }
      if (obj_list.objects[i].value == 0) {
        cold("PS1");
      } else {
        warm("PS1");
      }
    } else if (obj_list.objects[i].item == "Power2") {
      psu2_slider.value = obj_list.objects[i].value;
      if (obj_list.objects[i].value >= 0) {
        flip("B7", 0);
      } else {
        flip("B7", 180);
      }
      if (obj_list.objects[i].value == 0) {
        cold("PS2");
      } else {
        warm("PS2");
      }
    } else if (obj_list.objects[i].item.charAt(0) == 'T' && obj_list.objects[i].item.length == 2) {
      if (obj_list.objects[i].value == "closed") {
        document.getElementById("T"+obj_list.objects[i].item.charAt(1)+"_S").style.visibility="visible";
        document.getElementById("T"+obj_list.objects[i].item.charAt(1)+"_T").style.visibility="hidden";
      } else {
        document.getElementById("T"+obj_list.objects[i].item.charAt(1)+"_S").style.visibility="hidden";
        document.getElementById("T"+obj_list.objects[i].item.charAt(1)+"_T").style.visibility="visible";
      }
    } else if (obj_list.objects[i].item == "train1Pos") {
      document.getElementById(Train1Position).style.visibility="hidden";
      Train1Position = obj_list.objects[i].value+"_loco";
      document.getElementById(Train1Position).style.visibility="visible";
    } else if (obj_list.objects[i].item == "train2Pos") {
      document.getElementById(Train2Position).style.visibility="hidden";
      Train2Position = obj_list.objects[i].value+"_loco";
      document.getElementById(Train2Position).style.visibility="visible";
    }
  } /* list iteration */
} /* function UpdatePage */

