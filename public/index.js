
let currentStatusArray = [];

window.onbeforeunload = function(){
  currentStatusArray = tableToArray();
}

function myClearFunction(tableName){
    var tableHeaderRowCount = 1;
    var table = document.getElementById(tableName);
    var rowCount = table.rows.length;
    for (var i = tableHeaderRowCount; i < rowCount; i++) {
        table.deleteRow(tableHeaderRowCount);
    }
}

function myAddFunction() {
    var table = document.getElementById("statusTable");
    var row = table.insertRow(-1);
    var cell1 = row.insertCell(0);
    var cell2 = row.insertCell(1);
    cell1.innerHTML = "NEW CELL1";
    cell2.innerHTML = "NEW CELL2";
}

let getDropDownFile = function() {
    let e = document.getElementById("fileDropDown");
    let strUser = e.options[e.selectedIndex].text;
    return strUser;
}

function statusAdder(fileName, message){
    let table = document.getElementById("statusTable");
    col1 = $("<td>"+fileName+"</td>");
    col2 = $("<td>"+message+"</td>");
    $("<tr></tr>").append(col1, col2).prependTo(table);
}


function getStatusLog(){
  let currentStatusTable = document.getElementById("fileDropDown");
  return statusArray;
}

function tableToArray(){
    let myTable = document.getElementById('statusTable');
    //let arr = [];
    //currentStatusArray = [];
    let arr = [];
    for (var i = 1, row; row = myTable.rows[i]; i++) {
        let c1 = row.cells[0];
        let c2 = row.cells[1];
        var rowObj = {filename:c1.innerHTML, message:c2.innerHTML};

        arr.push(rowObj);
        //currentStatusArray.push(rowObj);
     }

     console.log(arr);
     return arr;
}


//basically on change add the file uploaded successfully to the array
//returned by get status log
function addUploadToArray(){
  //let currentStatusArray = tableToArray();
  let currentStatusArray = tableToArray();
  let uploadBar = document.getElementById('myFile');
  let uploadName = uploadBar.value;
  let uploadObj = {filename:uploadName, message:'Uploaded Sucessfully'};
  currentStatusArray.unshift(uploadObj);
  console.log(currentStatusArray);

  /*
  document.getElementById('myFile').onchange = function () {
    let rowobj = {}
    alert('Selected file: ' + this.value);
  };*/
  return currentStatusArray;
}

function uploadToStatus(){
  document.getElementById('myFile').onchange = function () {
    let array = addUploadToArray();
    for(let obj of array){
      let c1 = obj.filename;
      let c2 = obj.message;


      var table = document.getElementById("statusTable");
      var row = table.insertRow(-1);
      var cell1 = row.insertCell(0);
      var cell2 = row.insertCell(1);

      cell1.innerHTML = c1;
      //console.log(cell1.innerHTML);
      cell2.innerHTML = c2;
    }
  };
}



  $(document).on("click", "a", function() {
    //this == the link that was clicked
    //let fname = this;
    //let x = this.getAttribute('value');
    let mk = $(this).attr("href");
    let fileName = mk.substring(mk.lastIndexOf("/")+1);
    statusAdder(fileName, "File Successfully Downloaded");
    console.log(this);
  });



function displayCard(){
  let dropName = getDropDownFile();
  console.log(dropName);
  statusAdder(dropName, "Currently active in card view panel");

  $.ajax({
      type: 'get',            //Request type
      dataType: 'json',       //Data type - we will use JSON for almost everything
      data: {info: dropName},
      url: '/generateCardView',   //The server endpoint we are connecting to
      success: function (data) {
          myClearFunction("viewCardTable");
          let count = 0;
          for (array of data){
            for (property of array) {
              count++;
              let group = property.group;
              let name = property.name;
              let values = property.values;
              let stringValues = values.toString();
              //console.log(property.group);
              console.log(name);
              console.log(stringValues);
              var table = document.getElementById("viewCardTable");
              var row = table.insertRow(-1);
              var cell1 = row.insertCell(0);
              var cell2 = row.insertCell(1);
              var cell3 = row.insertCell(2);
              var cell4 = row.insertCell(3);

              cell1.innerHTML = count;
              cell2.innerHTML = group;
              cell3.innerHTML = name;
              cell4.innerHTML = stringValues;

            }

          }

          $('#cardViewTest').html("FileSummary test" + JSON.stringify(data));
          //We write the object to the console to show that the request was successful
          console.log(data);

      },
      fail: function(error) {
          // Non-200 return, do something with error
          console.log(error);
      }
  });
}



// Put all onload AJAX calls here, and event listeners
$(document).ready(function() {
  //tableToArray();
  //console.log(currentStatusArray);
    // On page-load AJAX Example
    $.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything
        url: '/someendpoint',   //The server endpoint we are connecting to
        success: function (data) {
            /*  Do something with returned object
                Note that what we get is an object, not a string,
                so we do not need to parse it on the server.
                JavaScript really does handle JSONs seamlessly
            */
            $('#blah').html("On page load, Received string '"+JSON.stringify(data)+"' from server");
            //We write the object to the console to show that the request was successful
            console.log(data);

        },
        fail: function(error) {
            // Non-200 return, do something with error
            console.log(error);
        }
    });


    $.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything
        url: '/fileNames',   //The server endpoint we are connecting to
        success: function (data) {
            for(let i = 0; i < data.length; i++){
              let file = data[i];
              $('#fileDropDown').append("<option onclick='displayCard()'>"+file+"</option>");
            }
            $('#fileDrop').html("Yoo coming from the server sidesss" + JSON.stringify(data));
            //We write the object to the console to show that the request was successful
            console.log(data);

        },
        fail: function(error) {
            // Non-200 return, do something with error
            console.log(error);
        }
    });



    $.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything
        url: '/fileLogDisplay',   //The server endpoint we are connecting to
        success: function (data) {
            myClearFunction('fileLogTable');

            for(let obj of data){
              let name = obj.name;
              let fullFilePath = obj.filename;
              let fileName = fullFilePath.substring(fullFilePath.lastIndexOf("/")+1);
              let numProperties = obj.additionalValues;

              /*console.log(obj.name);
              console.log(fileName);
              console.log(obj.additionalValues);*/

              var table = document.getElementById("fileLogTable");
              var row = table.insertRow(-1);
              var cell1 = row.insertCell(0);
              var cell2 = row.insertCell(1);
              var cell3 = row.insertCell(2);

              cell1.innerHTML = '<a href="/uploads/'+fileName+'">'+fileName+'</a>';
              //console.log(cell1.innerHTML);
              cell2.innerHTML = name;
              cell3.innerHTML = numProperties;
            }

            //$('#fileLogStuff').html("Yoo coming from the server sidesss forFILELOG" + JSON.stringify(data));
            //We write the object to the console to show that the request was successful
            console.log(data);

        },
        fail: function(error) {
            // Non-200 return, do something with error
            console.log(error);
        }
    });


    // Event listener form replacement example, building a Single-Page-App, no redirects if possible
    $('#someform').submit(function(e){
        $('#blah').html("Callback from the form");
        e.preventDefault();
        //Pass data to the Ajax call, so it gets passed to the
        $.ajax({});
    });
});
