
var idle = Boolean;

document.getElementById("fase1_b").addEventListener("click", function () {
    if (idle){
        idle = false;
        var xhttp = new XMLHttpRequest();
        xhttp.open("GET", "/fase1", true);
        xhttp.onreadystatechange = function () {
            if (xhttp.readyState == XMLHttpRequest.DONE) {
                if (xhttp.status != 200) {
                    // Error response
                    console.error("Error");
                }
            }
        };
        xhttp.send();
        document.getElementById("fase1_b").style.backgroundColor = "#daccae";
    } 
});

document.getElementById("fase2_b").addEventListener("click", function () {
    if (idle == true){
        idle = false;
        var xhttp = new XMLHttpRequest();
        xhttp.open("GET", "/fase2", true);
        xhttp.onreadystatechange = function () {
            if (xhttp.readyState == XMLHttpRequest.DONE) {
                if (xhttp.status != 200) {
                    console.error("Error");
                }
            }
        };
        xhttp.send();
        document.getElementById("fase2_b").style.backgroundColor = "#daccae";
    }

});

document.getElementById("fase3_b").addEventListener("click", function () {
    if (idle){
        idle = false;
        var xhttp = new XMLHttpRequest();
        xhttp.open("GET", "/fase3", true);
        xhttp.onreadystatechange = function () {
            if (xhttp.readyState == XMLHttpRequest.DONE) {
                if (xhttp.status != 200) {
                    console.error("Error");
                }
            }
        };
        xhttp.send();
        document.getElementById("fase3_b").style.backgroundColor = "#daccae";
    }
});

document.getElementById("boton-secundario").addEventListener("click", function () {
    // Post request to dispense endpoint
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "/results", true);
    xhttp.onreadystatechange = function () {
        if (xhttp.readyState == XMLHttpRequest.DONE) {
            if (xhttp.status == 200) {
                console.log(xhttp.responseText);
                var response = xhttp.responseText;
                var response_array = response.split(',');
            }
        }
    };
    xhttp.send();

    document.getElementById("long_paso_res").innerHTML = response_array[0];          //actualizacion de parametros
    document.getElementById("long_zan_res").innerHTML = response_array[1];           
    document.getElementById("tiempo_res").innerHTML = "4";
    document.getElementById("vel_res").innerHTML = "5";

    document.getElementById("fase1_b").style.backgroundColor = "#f0e1c0";
    document.getElementById("fase2_b").style.backgroundColor = "#f0e1c0";
    document.getElementById("fase3_b").style.backgroundColor = "#f0e1c0";
    idle = true;
});
