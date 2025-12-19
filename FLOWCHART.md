# FLOWCHART: Box Security Monitoring System (ESP32)

## 🔄 Complete System Flowchart

```mermaid
graph TD
    Start([START: Power ON]) --> Init[Initialize System]
    Init --> InitSerial[Serial.begin 115200]
    InitSerial --> InitPins[Setup GPIO Pins<br/>Hall: GPIO 16<br/>Tilt: GPIO 17]
    InitPins --> PrintHeader[Print System Header]
    PrintHeader --> ConnectWiFi{Connect WiFi}
    
    ConnectWiFi -->|Success| SetupTelegram[Setup Telegram<br/>setCACert]
    ConnectWiFi -->|Failed| RetryWiFi[Wait & Retry]
    RetryWiFi --> ConnectWiFi
    
    SetupTelegram --> SendStartup[Send Startup Message<br/>to Telegram]
    SendStartup --> SystemReady[System Ready<br/>Print Status]
    SystemReady --> MainLoop[Enter Main Loop]
    
    %% Main Loop
    MainLoop --> CheckWiFi{WiFi<br/>Connected?}
    CheckWiFi -->|No - Every 30s| ReconnectWiFi[Reconnect WiFi]
    CheckWiFi -->|Yes| CheckBotInterval{Time to Check<br/>Telegram?}
    ReconnectWiFi --> CheckBotInterval
    
    CheckBotInterval -->|Yes - Armed: 1s<br/>Sleep: 5s| HandleTelegram[Handle Telegram<br/>Messages]
    CheckBotInterval -->|No| CheckArmed{System<br/>Armed?}
    HandleTelegram --> CheckArmed
    
    %% Armed Mode Branch
    CheckArmed -->|Yes - ARMED| ReadSensors[Read Sensors<br/>Hall & Tilt]
    ReadSensors --> DisplaySerial[Display Status<br/>to Serial Monitor]
    DisplaySerial --> CheckWiFiArmed{WiFi OK?}
    
    CheckWiFiArmed -->|Yes| CheckBoxState[Check Box Opened<br/>Hall Sensor]
    CheckWiFiArmed -->|No| DelaySensor[delay 50ms]
    
    CheckBoxState --> CheckBreach{Security<br/>Breached?}
    CheckBreach -->|Yes| BreachInterval{Time for<br/>Breach Alert?}
    BreachInterval -->|Yes - Every 30s| SendBreachAlert[Send Security<br/>Breach Alert]
    BreachInterval -->|No| CheckBoxOpen
    SendBreachAlert --> CheckBoxOpen
    
    CheckBreach -->|No| CheckBoxOpen{Box<br/>Open?}
    CheckBoxOpen -->|Yes| OpenInterval{Time for<br/>Open Alert?}
    OpenInterval -->|Yes - Every 30s| SendOpenAlert[Send Box<br/>Opened Alert]
    OpenInterval -->|No| CheckMotion
    SendOpenAlert --> CheckMotion
    CheckBoxOpen -->|No| CheckMotion
    
    CheckMotion[Check Box Movement<br/>Tilt Sensor] --> DelaySensor
    DelaySensor --> MainLoop
    
    %% Sleep Mode Branch
    CheckArmed -->|No - SLEEP| SleepLog{10s<br/>Elapsed?}
    SleepLog -->|Yes| PrintSleepLog[Print Sleep Mode<br/>Message]
    SleepLog -->|No| DelaySleep
    PrintSleepLog --> DelaySleep[delay 1000ms]
    DelaySleep --> MainLoop
```

---

## 📱 Telegram Message Handler Flowchart

```mermaid
graph TD
    HandleStart([Handle Telegram Messages]) --> GetUpdates[Get Updates from<br/>Telegram API]
    GetUpdates --> CheckResponse{Response<br/>OK?}
    
    CheckResponse -->|No| HandleEnd([Return])
    CheckResponse -->|Yes| HasMessages{Has New<br/>Messages?}
    HasMessages -->|No| HandleEnd
    
    HasMessages -->|Yes| LoopMessages[Loop Through<br/>Messages]
    LoopMessages --> CheckChatID{Chat ID<br/>Valid?}
    
    CheckChatID -->|No - Unauthorized| SendUnauth[Send Unauthorized<br/>Message]
    SendUnauth --> NextMessage{More<br/>Messages?}
    
    CheckChatID -->|Yes| ParseCommand{Parse<br/>Command}
    
    ParseCommand -->|/start or /help| SendHelp[Send Help Menu]
    ParseCommand -->|/status| SendStatus[Send System Status<br/>Comprehensive Info]
    ParseCommand -->|/arm| ArmSystem[Set isSystemArmed = true<br/>Send Confirmation]
    ParseCommand -->|/disarm or /sleep| DisarmSystem[Set isSystemArmed = false<br/>Send Sleep Message]
    ParseCommand -->|/safe| CheckBreachFlag{Breach<br/>Active?}
    ParseCommand -->|Unknown| SendUnknown[Send Unknown<br/>Command Message]
    
    CheckBreachFlag -->|Yes| ResetBreach[Reset securityBreached<br/>Send Safe Confirmation]
    CheckBreachFlag -->|No| SendNoBreach[Send No Breach<br/>Message]
    
    SendHelp --> NextMessage
    SendStatus --> NextMessage
    ArmSystem --> NextMessage
    DisarmSystem --> NextMessage
    ResetBreach --> NextMessage
    SendNoBreach --> NextMessage
    SendUnknown --> NextMessage
    
    NextMessage -->|Yes| LoopMessages
    NextMessage -->|No| HandleEnd
```

---

## 🚪 Box Opened Detection Flowchart

```mermaid
graph TD
    CheckBox([Check Box Opened]) --> ReadHall[Read Hall Sensor Value]
    ReadHall --> CheckInverted{Sensor<br/>Inverted?}
    
    CheckInverted -->|Yes| InvertedLogic[HIGH = Closed<br/>LOW = Opened]
    CheckInverted -->|No| NormalLogic[LOW = Closed<br/>HIGH = Opened]
    
    InvertedLogic --> CompareState{State<br/>Changed?}
    NormalLogic --> CompareState
    
    CompareState -->|No| BoxEnd([Return])
    CompareState -->|Yes - New State| CheckOpened{Box<br/>Opened?}
    
    CheckOpened -->|Yes - OPENED| SetFlags[Set isBoxOpen = true<br/>Set securityBreached = true]
    SetFlags --> PrintBreach[Print SECURITY<br/>BREACHED Message]
    PrintBreach --> SendFirst[Send Box Opened Alert<br/>First Time]
    SendFirst --> UpdateState[Update lastHallState]
    UpdateState --> BoxEnd
    
    CheckOpened -->|No - CLOSED| ClearOpen[Set isBoxOpen = false]
    ClearOpen --> PrintClosed[Print Box Closed<br/>Message]
    PrintClosed --> LogSerial[Log to Serial Only<br/>No Telegram Notification]
    LogSerial --> UpdateState
```

---

## 📐 Motion Detection Flowchart

```mermaid
graph TD
    CheckMotion([Check Box Movement]) --> ReadTilt[Read Tilt Sensor Value]
    ReadTilt --> CompareState{Tilt State<br/>Changed?}
    
    CompareState -->|No| CheckStable{Motion<br/>Active?}
    CompareState -->|Yes - MOTION!| UpdateTime[Update lastTiltChangeTime]
    
    UpdateTime --> CheckActive{Motion<br/>Currently Active?}
    CheckActive -->|No - First Motion| SetActive[Set isMotionActive = true<br/>Reset motionAlertSent]
    CheckActive -->|Yes - Ongoing| CheckCooldown
    
    SetActive --> PrintMotion[Print MOTION<br/>DETECTED Message]
    PrintMotion --> CheckCooldown{Alert Sent<br/>or Cooldown?}
    
    CheckCooldown -->|No Alert Yet| SendMotionAlert[Send Motion Alert<br/>to Telegram]
    CheckCooldown -->|Cooldown Passed - 5s| SendMotionAlert
    CheckCooldown -->|Wait Cooldown| UpdateTiltState
    
    SendMotionAlert --> UpdateTiltState[Update lastTiltState<br/>Set motionAlertSent]
    UpdateTiltState --> MotionEnd([Return])
    
    CheckStable -->|Yes| CalcStableTime[Calculate Time Since<br/>Last Change]
    CheckStable -->|No| MotionEnd
    
    CalcStableTime --> CheckThreshold{Stable > 3s?}
    CheckThreshold -->|Yes - STABLE| StopMotion[Set isMotionActive = false<br/>Reset motionAlertSent]
    CheckThreshold -->|No| MotionEnd
    
    StopMotion --> PrintStopped[Print Motion Stopped<br/>Message]
    PrintStopped --> LogStoppedSerial[Log to Serial Only<br/>No Telegram Notification]
    LogStoppedSerial --> MotionEnd
```

---

## 🔐 System State Diagram

```mermaid
stateDiagram-v2
    [*] --> Initializing: Power ON
    
    Initializing --> Armed: Setup Complete<br/>Default Mode
    
    Armed --> Sleep: /disarm or /sleep<br/>Command
    Sleep --> Armed: /arm Command
    
    Armed --> BreachActive: Box Opened<br/>OR Motion Detected
    
    BreachActive --> BreachActive: Alert Every 30s<br/>Until /safe
    BreachActive --> Armed: /safe Command<br/>Reset Breach Flag
    
    Sleep --> Sleep: Sensor NOT Read<br/>WiFi Active<br/>Bot Check 5s
    Armed --> Armed: Sensor Read 50ms<br/>Bot Check 1s
    
    note right of Armed
        ✅ Read sensors
        ✅ Send alerts
        ✅ Monitor box
        ⚡ High power
    end note
    
    note right of Sleep
        ❌ No sensor read
        ✅ WiFi connected
        ✅ Receive commands
        🔋 Low power
    end note
    
    note right of BreachActive
        🚨 Persistent alerts
        📢 Until /safe sent
        🔓 Security compromised
    end note
```

---

## 🔄 Alert Flow Decision Tree

```mermaid
graph TD
    AlertStart([Alert Trigger]) --> CheckArmed{System<br/>Armed?}
    
    CheckArmed -->|No - SLEEP| NoAlert[No Alert Sent<br/>Sensors Not Read]
    CheckArmed -->|Yes - ARMED| CheckWiFi{WiFi<br/>Connected?}
    
    CheckWiFi -->|No| LogOnly[Log to Serial<br/>Only]
    CheckWiFi -->|Yes| CheckEventType{Event<br/>Type?}
    
    CheckEventType -->|Box Opened| CheckBreachFlag{Security<br/>Breached?}
    CheckEventType -->|Motion| CheckMotionActive{Motion<br/>Active?}
    CheckEventType -->|Box Closed| SilentLog[Silent - Log Only<br/>No Alert]
    CheckEventType -->|Motion Stopped| SilentLog
    
    CheckBreachFlag -->|Yes - PRIORITY| CheckBreachInterval{Breach Alert<br/>Interval OK?}
    CheckBreachInterval -->|Yes - 30s passed| SendBreachAlert[🚨 Send Security<br/>Breach Alert]
    CheckBreachInterval -->|No - Wait| NoAlert
    
    CheckBreachFlag -->|No - First Time| SendOpenAlert[📬 Send Box<br/>Opened Alert]
    SendOpenAlert --> SetBreachFlag[Set securityBreached<br/>= true]
    
    CheckMotionActive -->|First Motion| SendMotionAlert[⚠️ Send Motion<br/>Alert]
    CheckMotionActive -->|Ongoing| CheckMotionCooldown{Cooldown<br/>5s Passed?}
    CheckMotionCooldown -->|Yes| SendMotionAlert
    CheckMotionCooldown -->|No| NoAlert
    
    SendBreachAlert --> AlertEnd([End])
    SetBreachFlag --> AlertEnd
    SendMotionAlert --> AlertEnd
    SilentLog --> AlertEnd
    LogOnly --> AlertEnd
    NoAlert --> AlertEnd
```

---

## 📊 Timing & Intervals Diagram

```mermaid
gantt
    title System Timing Intervals
    dateFormat X
    axisFormat %L ms
    
    section Armed Mode
    Sensor Read (50ms)           :0, 50
    Bot Check (1000ms)           :0, 1000
    Motion Stable Check (3000ms) :0, 3000
    Box Open Alert (30000ms)     :0, 30000
    Motion Alert Cooldown (5000ms) :0, 5000
    WiFi Reconnect Check (30000ms) :0, 30000
    
    section Sleep Mode
    Bot Check (5000ms)           :0, 5000
    Delay Loop (1000ms)          :0, 1000
    Sleep Log Print (10000ms)    :0, 10000
    WiFi Reconnect Check (30000ms) :0, 30000
```

---

## 🎯 Complete System Architecture

```mermaid
graph TB
    subgraph Hardware
        ESP[ESP32 Board]
        Hall[KY-003 Hall Sensor<br/>GPIO 16]
        Tilt[KY-027 Tilt Sensor<br/>GPIO 17]
        Power[Power Supply<br/>5V USB]
    end
    
    subgraph Software_Modules
        Main[Main Loop<br/>Box-Security-Monitoring-Esp32.ino]
        Config[Configuration<br/>config.h]
        WiFiMgr[WiFi Manager<br/>wifi_manager.h]
        SensorMgr[Sensor Handler<br/>sensor_handler.h]
        TelegramMgr[Telegram Manager<br/>telegram_manager.h]
        Utils[Utilities<br/>utils.h]
    end
    
    subgraph External_Services
        WiFiNet[WiFi Network<br/>2.4GHz]
        TelegramAPI[Telegram Bot API<br/>HTTPS]
        UserPhone[User Smartphone<br/>Telegram App]
    end
    
    subgraph System_States
        Armed[ARMED Mode<br/>Full Monitoring]
        Sleep[SLEEP Mode<br/>Power Saving]
        Breach[BREACH State<br/>Alert Active]
    end
    
    Power --> ESP
    Hall --> ESP
    Tilt --> ESP
    
    ESP --> Main
    Main --> Config
    Main --> WiFiMgr
    Main --> SensorMgr
    Main --> TelegramMgr
    Main --> Utils
    
    WiFiMgr --> WiFiNet
    TelegramMgr --> TelegramAPI
    TelegramAPI --> UserPhone
    
    Main --> Armed
    Main --> Sleep
    Armed --> Breach
    
    SensorMgr --> Hall
    SensorMgr --> Tilt
    
    UserPhone -.->|Commands| TelegramAPI
    TelegramAPI -.->|Alerts| UserPhone
```

---

## 🔢 Code Execution Flow (Detailed)

```mermaid
sequenceDiagram
    participant User
    participant Telegram
    participant ESP32
    participant Sensors
    participant Serial
    
    Note over ESP32: STARTUP SEQUENCE
    ESP32->>Serial: Print Header
    ESP32->>Sensors: Initialize Pins
    ESP32->>ESP32: Connect WiFi
    ESP32->>Telegram: Setup SSL Certificate
    ESP32->>Telegram: Send Startup Message
    Telegram->>User: 🤖 System Active
    
    Note over ESP32: MAIN LOOP (ARMED)
    loop Every 50ms
        ESP32->>Sensors: Read Hall & Tilt
        Sensors-->>ESP32: Sensor Values
        ESP32->>Serial: Display Status
        
        alt Box Opened Detected
            ESP32->>ESP32: Set Breach Flag
            ESP32->>Telegram: Send Open Alert
            Telegram->>User: 🚨 Box Opened!
        end
        
        alt Motion Detected
            ESP32->>Telegram: Send Motion Alert
            Telegram->>User: ⚠️ Motion Detected!
        end
    end
    
    loop Every 1s (Armed)
        ESP32->>Telegram: Check Messages
        User->>Telegram: /status
        Telegram->>ESP32: Forward Command
        ESP32->>Telegram: Send Status Info
        Telegram->>User: 📊 System Status
    end
    
    User->>Telegram: /sleep
    Telegram->>ESP32: Sleep Command
    
    Note over ESP32: SLEEP MODE
    ESP32->>Serial: Sleep Mode Log
    ESP32->>ESP32: Skip Sensor Reading
    
    loop Every 5s (Sleep)
        ESP32->>Telegram: Check Messages
    end
    
    User->>Telegram: /arm
    Telegram->>ESP32: Arm Command
    
    Note over ESP32: BACK TO ARMED MODE
    ESP32->>Telegram: System Armed
    Telegram->>User: ✅ Armed
```

---

## 📋 Summary

### Key Components:
1. **Initialization**: GPIO setup, WiFi connection, Telegram setup
2. **Main Loop**: Armed vs Sleep mode decision
3. **Sensor Reading**: Hall (box open) & Tilt (motion) detection
4. **Alert System**: Persistent breach alerts, motion alerts
5. **Telegram Integration**: Remote control and notifications
6. **Power Management**: Sleep mode for energy efficiency

### Critical Paths:
- **Breach Detection**: Box opened → Set flag → Alert until /safe
- **Motion Detection**: Tilt change → Alert → Wait stable (3s)
- **Sleep Mode**: No sensor read → WiFi active → Command response only
- **WiFi Management**: Auto-reconnect every 30s if disconnected

### Timing Constants:
- Armed sensor read: **50ms**
- Armed bot check: **1s**
- Sleep bot check: **5s**
- Motion stable: **3s**
- Alert interval: **30s**
- Motion cooldown: **5s**
