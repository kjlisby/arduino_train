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
//	document.getElementById("demo").innerHTML = angle_str;
	var angle = Number(angle_str);
//	document.getElementById("demo").innerHTML = angle;
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
