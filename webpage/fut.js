var i = 0
function myFunction() {
  document.getElementById("demo").innerHTML = "Hello JavaScript!"+i
  i++
}

function flip(idToFlip) {
	var elementToFlip = document.getElementById(idToFlip);
	var transform_string = elementToFlip.style.transform;
	var angle_str = "0";
	if (transform_string.indexOf("otate(") > 0) {
		pos1 = transform_string.indexOf("(")+1;
		pos2 = transform_string.indexOf("deg")
		angle_str = transform_string.slice(pos1, pos2);
	}
	var angle = Number(angle_str);
	if (angle < 180) {
		angle = angle + 180;
	} else {
		angle = angle - 180;
	}
	document.getElementById("demo").innerHTML = angle;
	elementToFlip.style.transform = "rotate("+angle+"deg)";
}

function warm(myId) {
	document.getElementById(myId).src = "skinne_varm.gif";
}

function cold(myId) {
	document.getElementById(myId).src = "skinne_kold.jpg";
}

var slider = document.getElementById("Power1");
var output = document.getElementById("demo");
slider.oninput = function() {
  setPower1(this.value);
}
document.getElementById('reset1').onclick = function(){
 document.getElementById('Power1').value = 0;
 setPower1(slider.value);
}
function setPower1(value) {
 /*
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      power1set(value);
    }
  };
  xhttp.open("GET", "ajax_set_power1_"+value, true);
  xhttp.send();
  */
  output.innerHTML = value;
}


var t1s = document.getElementById("T1_S");
var t2s = document.getElementById("T2_S");
var t1t = document.getElementById("T1_T");
var t2t = document.getElementById("T2_T");
var b1 = document.getElementById("B1");
var b2 = document.getElementById("B2");
t1s.onclick = function(){
 activateB2();
}
t2s.onclick = function(){
 activateB2();
}
t1t.onclick = function(){
 activateB1();
}
t2t.onclick = function(){
 activateB1();
}
b1.onclick = function(){
 activateB1();
}
b2.onclick = function(){
 activateB2();
}
function activateB1() {
/*  call the ESP32 to activate B1 */

  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      B1activated();
    }
  };
  xhttp.open("GET", "ajax_set_B1", true);
  xhttp.send();

 /*
 B1activated();
 */
}
function activateB2() {
/*  call the ESP32 to activate B2 */

  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      B2activated();
    }
  };
  xhttp.open("GET", "ajax_set_B2", true);
  xhttp.send();

 /*
 B2activated();
 */
}
function B1activated() {
 output.innerHTML = "B1 active";
 t1t.style.visibility="hidden";
 t2t.style.visibility="hidden";
 t1s.style.visibility="visible";
 t2s.style.visibility="visible";
 b1.src = "skinne_varm.gif";
 b2.src = "skinne_kold.jpg";
}
function B2activated() {
 output.innerHTML = "B2 active";
 t1s.style.visibility="hidden";
 t2s.style.visibility="hidden";
 t1t.style.visibility="visible";
 t2t.style.visibility="visible";
 b2.src = "skinne_varm.gif";
 b1.src = "skinne_kold.jpg";
}

var autobox       = document.getElementById("automode");
var automodelabel = document.getElementById("automodelabel");
function autoCheckbox() {
 if (autobox.checked == true) {
	 automodelabel.style.color = "red";
 } else {
	 automodelabel.style.color = "black";
 }
 /*  call the ESP32 to activate auto mode

  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      
    }
  };
  xhttp.open("GET", "ajax_set_automode_"+autobox.checked, true);
  xhttp.send();

 */
}