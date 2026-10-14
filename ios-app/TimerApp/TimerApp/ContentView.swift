

import SwiftUI

struct ContentView: View {

    @State private var isRunning = false
    @State private var showAlert = false
    @State private var alertMessage = ""

    var body: some View {

        Button(isRunning ? "STOP" : "START") {

            if !isRunning {
                startTimer()
            } else {
                stopTimer()
            }

        }
        .padding()
        .font(.default)
        .bold()
        .background(isRunning ? Color.red : Color.green)
        .cornerRadius(16)
        .foregroundColor(.white)

        .alert("Timer", isPresented: $showAlert) {
            Button("OK") {
                isRunning = false
            }
        } message: {
            Text(alertMessage)
        }
    }

    func startTimer() {
        TimerWrapper.start()
        isRunning = true
    }

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
