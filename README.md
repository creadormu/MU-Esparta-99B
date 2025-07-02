# Mu Esparta 99B - Bots Inteligentes para MU Online

Proyecto en C++ para el servidor **Mu Esparta 99B**, que implementa un sistema avanzado de **bots simuladores de jugadores reales** dentro del mundo MU Online. Estos bots no solo caminan y juegan, sino que también **hablan automáticamente** en diferentes situaciones como si fueran jugadores humanos.

> ⚔️ Inspirado en MU Online 99B — modificado para añadir realismo con IA básica y comportamiento simulado en bots.

---

## 🧠 Características principales

- 🤖 **Bots activos** que usan cuentas de usuarios no conectados.
- 💬 **Sistema de diálogo inteligente**:
  - Frases aleatorias desde archivo externo.
  - Frases específicas dependiendo del entorno o la situación (por ejemplo, si hay jugadores cerca).
  - Respuestas automáticas ante vulgaridades detectadas (por ejemplo: “mierda”, “carajo”).
- 🎮 **Comportamiento jugable**:
  - Los bots caminan, se mueven por el mapa y simulan actividad real.
  - Pueden **invitar a party** a usuarios cercanos automáticamente.
- 📝 Frases configurables desde archivos `.txt`, sin necesidad de recompilar.
- 🔐 Implementación segura con uso de mutex para evitar conflictos entre hilos.

---

## 🛠 Requisitos

- C++11 o superior.
- Compilador compatible: MSVC (Visual Studio) o g++.
- Sistema operativo: Windows.
- Proyecto base MuServer 99B (versión personalizada).

---

## 🚀 Instalación y uso

1. Clona el repositorio:

```bash
git clone https://github.com/creadormu/MU-Esparta-99B.git

