import React, { useState } from 'react';
import { Alert, ScrollView, StyleSheet, Text, TouchableOpacity, View } from 'react-native';

// Lista de candidatos ficticia
const CANDIDATOS = [
  { id: 1, nombre: 'Cuchu', partido: 'La Cámpora' },
  { id: 2, nombre: 'Javier Miguel', partido: 'La Libertad Avanza' },
  { id: 3, nombre: 'LA REINA', partido: 'Critina libree' },
  { id: 4, nombre: 'Javier Toobe', partido: 'ESP Lovers' },
  { id: 5, nombre: 'Momo', partido: 'Platense' },
  { id: 6, nombre: 'Voto en Blanco', partido: '-' },
];

export default function HomeScreen() {
  // Ahora sumamos el estado 'votando'
  const [status, setStatus] = useState<'esperando' | 'habilitado' | 'bloqueado' | 'votando'>('esperando');

  const manejarVoto = (nombreCandidato: string) => {
    Alert.alert(
      "Confirmación",
      `¿Desea confirmar su voto para: ${nombreCandidato}?`,
      [
        { text: "Cancelar", style: "cancel" },
        { 
          text: "Confirmar", 
          onPress: () => {
            Alert.alert("Éxito", "Voto registrado correctamente. Gracias por participar.");
            setStatus('esperando'); // Volvemos al inicio
          } 
        }
      ]
    );
  };

  return (
    <View style={styles.container}>
      <Text style={styles.header}>SISTEMA ELECTORAL</Text>

      {/* 1. ESPERANDO ESCANEO */}
      {status === 'esperando' && (
        <View style={styles.card}>
          <Text style={styles.infoText}>Esperando escaneo de DNI en el puesto físico...</Text>
        </View>
      )}

      {/* 2. BLOQUEADO POR ANTECEDENTES */}
      {status === 'bloqueado' && (
        <View style={[styles.card, styles.cardError]}>
          <Text style={styles.errorTitle}>⚠️ ACCESO DENEGADO</Text>
          <Text style={styles.errorText}>Se han detectado antecedentes penales vigentes.</Text>
          <TouchableOpacity onPress={() => setStatus('esperando')} style={styles.btnVolver}>
             <Text>Volver al inicio</Text>
          </TouchableOpacity>
        </View>
      )}

      {/* 3. HABILITADO (Botón para entrar al cuarto oscuro) */}
      {status === 'habilitado' && (
        <TouchableOpacity 
          style={styles.botonIngresar} 
          onPress={() => setStatus('votando')}
        >
          <Text style={styles.botonTexto}>INGRESAR AL CUARTO OSCURO</Text>
        </TouchableOpacity>
      )}

      {/* 4. PANTALLA DE VOTACIÓN (Elección de candidatos) */}
      {status === 'votando' && (
        <ScrollView style={{ width: '100%' }}>
          <Text style={styles.subTitulo}>Seleccione su candidato:</Text>
          {CANDIDATOS.map((c) => (
            <TouchableOpacity 
              key={c.id} 
              style={styles.cardCandidato} 
              onPress={() => manejarVoto(c.nombre)}
            >
              <Text style={styles.nombreCandidato}>{c.nombre}</Text>
              <Text style={styles.partidoCandidato}>{c.partido}</Text>
            </TouchableOpacity>
          ))}
        </ScrollView>
      )}

      {/* PANEL DE PRUEBA (SOLO PARA DESARROLLO) */}
      {status === 'esperando' && (
        <View style={styles.debugPanel}>
          <TouchableOpacity onPress={() => setStatus('habilitado')} style={styles.btnDebug}><Text>Simular OK</Text></TouchableOpacity>
          <TouchableOpacity onPress={() => setStatus('bloqueado')} style={styles.btnDebug}><Text>Simular Bloqueo</Text></TouchableOpacity>
        </View>
      )}
    </View>
  );
}

const styles = StyleSheet.create({
  container: { flex: 1, backgroundColor: '#f0f4f7', alignItems: 'center', justifyContent: 'center', padding: 20, paddingTop: 60 },
  header: { fontSize: 26, fontWeight: 'bold', color: '#1a237e', marginBottom: 30 },
  subTitulo: { fontSize: 18, marginBottom: 20, textAlign: 'center', fontWeight: '600' },
  card: { backgroundColor: 'white', padding: 30, borderRadius: 15, width: '100%', alignItems: 'center', elevation: 4 },
  infoText: { textAlign: 'center', fontSize: 16, color: '#546e7a' },
  cardError: { backgroundColor: '#ffebee', borderWidth: 2, borderColor: '#c62828' },
  errorTitle: { fontSize: 20, fontWeight: 'bold', color: '#c62828', marginBottom: 10 },
  errorText: { textAlign: 'center', color: '#b71c1c', marginBottom: 20 },
  botonIngresar: { backgroundColor: '#2e7d32', padding: 25, borderRadius: 12, width: '100%' },
  botonTexto: { color: 'white', fontWeight: 'bold', textAlign: 'center', fontSize: 18 },
  cardCandidato: { backgroundColor: 'white', padding: 20, borderRadius: 10, marginBottom: 15, borderWidth: 1, borderColor: '#ddd' },
  nombreCandidato: { fontSize: 18, fontWeight: 'bold' },
  partidoCandidato: { color: '#666' },
  btnVolver: { marginTop: 10, padding: 10, backgroundColor: '#ddd', borderRadius: 5 },
  debugPanel: { flexDirection: 'row', marginTop: 40 },
  btnDebug: { margin: 10, padding: 15, backgroundColor: '#cfd8dc', borderRadius: 8 }
});