// Constantes 
const express=require('express');
const bodyParser=require('body-parser');
const mysql=require('mysql');

const cors=require('cors');

let app=express();
app.use(cors());

app.use(bodyParser.urlencoded({ extended: true }));
app.use(bodyParser.json({type:'application/json'}));

const con = mysql.createConnection({
    host:'34.95.167.255',
    user:'root',
    password:'1234',
    database:'trabalho_iot'
});

const server = app.listen(3000, function(){
    var host = server.address().address;
    var port = server.address().port;
});

con.connect(function(error){
    if(error) console.log(error);
    else console.log('conectado');
})