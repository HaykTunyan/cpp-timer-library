

import SwiftUI

struct ContentView: View {

    // State variables for timer and alert.
    @State private var isRunning = false
    @State private var showAlert = false
    @State private var alertMessage = ""

    var body: some View {

        Button{
            if !isRunning {
                startTimer()
            } else {
                stopTimer()
                
            }
        } 
        label: { 
            Text(isRunning ? "STOP" : "START")
            .padding()
            .font(.default)
            .bold()
            .background(isRunning ? Color.red : Color.green)
            .cornerRadius(16)
            .foregroundColor(.white)
        }
        .alert("Timer", isPresented: $showAlert) {
            Button("OK") {
                isRunning = false
            }
        } 
        message: {
            Text(alertMessage)
        }
    }
    
    // Function to start timer.
    func startTimer() {
        TimerWrapper.start()
        isRunning = true
    }

    // Function to stop timer. 
    func stopTimer() {
        let seconds = TimerWrapper.stop()

        print("Swift received: \(seconds) seconds")

        alertMessage = "Прошло \(seconds) секунд"
        showAlert = true
    }
}

#Preview {
    ContentView()
}
