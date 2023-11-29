
import React, { useState } from 'react';
import {FlatList, StyleSheet, Text, View, TouchableOpacity } from 'react-native';
import {MaterialCommunityIcons} from '@expo/vector-icons';
//import config from "config/config.json";

export default function Feed() {
  const [iconColors, setIconColors] = useState({
    icon1: 'black',
    icon2: 'black',
    icon3: 'black',
    icon4: 'black',
  });

  const toggleIconColor = (id) => {
    // Crie uma cópia do objeto de cores e altere a cor do ícone clicado
    const updatedIconColors = { ...iconColors };
    var icone = "icon" + String(id)
    updatedIconColors[icone] = updatedIconColors[icone] === 'black' ? 'blue' : 'black';
    setIconColors(updatedIconColors);
    ligarLampada(id);
  };

  async function getEstadoLampada(id){
    let url = 'https://34.95.167.255/getLampadas';
    try{
        let reqs = await fetch(url ,{
            method: 'GET',
            headers:{
                "Accept":'application/json',
                "Content-Type":'application/json',
                'id': id
            }
        });
    } catch (e){
        console.log(e);
    }
  }
//
  function mudaEstado(id){
    if (getEstadoLampada(id) = 1) return 0;
    else return 1;
  }
//
  // envia dados para back end
  async function ligarLampada(id){
    let url = 'https://34.95.167.255/teste';
    try{
        let reqs = await fetch(url ,{
            method: 'POST',
            headers:{
                "Accept":'application/json',
                "Content-Type":'application/json',
            },body: JSON.stringify({
              idLampada: id,
              estadoLampada: mudaEstado(id),
            })
        });
    } catch (e){
        console.log(e);
    }
    
  }

  const DATA = [
    {
      key: 1,
      title: 'Sacada',
      icone: '1',
      cor: iconColors.icon1
    },
    {
      key: 2,
      title: 'Quarto',
      icone: '2',
      cor: iconColors.icon2
    },
    {
      key: 3,
      title: 'Banheiro',
      icone: '3',
      cor: iconColors.icon3
    },
    {
      key: 4,
      title: 'Cozinha',
      icone: "4",
      cor: iconColors.icon4
    },
  ];
  
  const Item = ({title, icone, cor}) => (
    <TouchableOpacity onPress={() => toggleIconColor(icone)}>
    <View style={styles.item}>
      <MaterialCommunityIcons name="lightbulb-outline" size={24} color={cor} />
      <Text style={styles.title}>{title}</Text>
    </View>
    </TouchableOpacity>
  );

  return (
    <View style={styles.mainContainer}>

      <View style={styles.homeContainer}>
        <View style={styles.segundoAndar}>
          <View style={styles.quarto}>
            <TouchableOpacity onPress={() => toggleIconColor('1')}>
              <MaterialCommunityIcons name="lightbulb-outline" size={24} color={iconColors.icon1} />
            </TouchableOpacity>
          </View>
          <View style={styles.quarto}>
            <TouchableOpacity onPress={() => toggleIconColor('2')}>
              <MaterialCommunityIcons name="lightbulb-outline" size={24} color={iconColors.icon2} />
            </TouchableOpacity>
          </View>
          <View style={styles.quarto}>
            <TouchableOpacity onPress={() => toggleIconColor('3')}>
              <MaterialCommunityIcons name="lightbulb-outline" size={24} color={iconColors.icon3} />
            </TouchableOpacity>
          </View>
        </View>
        <View style={styles.primeiroAndar}>
          <TouchableOpacity onPress={() => toggleIconColor('4')}>
            <MaterialCommunityIcons name="lightbulb-outline" size={24} color={iconColors.icon4} />
          </TouchableOpacity>
        </View>
      </View>


      <View style={styles.listContainer}>
      <FlatList
        data={DATA}
        renderItem={({item}) => <Item title={item.title} icone={item.icone} cor={item.cor} />}
        keyExtractor={(item, index) => {
          return  index.toString();
         }}
        extraData={this.state}
      />
      </View>
      
    </View>
  );
}

const styles = StyleSheet.create({
  mainContainer: {
    flex: 1,
    backgroundColor: '#fff',
    alignItems: 'center',
    justifyContent: 'center'
  },
  homeContainer:{
    flex:1,
    borderBottomWidth:1,
    alignItems: 'center',
    justifyContent: 'center',
    width: '100%'
  },
  primeiroAndar:{
    flex:0.3,
    width:'70%',
    borderWidth:1,
    alignItems: 'center',
    justifyContent: 'center',
  },
  segundoAndar:{
    flex:0.3,
    width:'70%',
    borderWidth:1,
    margin: 10,
    flexDirection: 'row',
  },
  quarto:{
    flex:1,
    borderWidth:0.5,
    alignItems: 'center',
    justifyContent: 'center',
  },
  listContainer:{
    flex:1,
    width: '100%',
  },
  item: {
    backgroundColor: '#f2f5f7',
    borderWidth: 1,
    borderRadius:5,
    borderColor: "#c8cacc",
    padding: 20,
    marginVertical: 8,
    marginHorizontal: 16,
    flexDirection: "row",
  },
  title: {
    fontSize: 22,
    fontWeight: 'bold'
  }
});
