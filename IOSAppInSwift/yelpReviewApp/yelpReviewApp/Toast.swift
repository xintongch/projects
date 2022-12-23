//
//  Toast.swift
//  yelp-9
//
//  Created by Xintong Chen on 12/3/22.
//

import SwiftUI
//import Alamofire
//import SwiftyJSON
import MapKit



extension View {
    func toast1<Content>(isPresented: Binding<Bool>, content: @escaping () -> Content) -> some View where Content: View {
        Toast1(
            isPresented: isPresented,
            presenter: { self },
            content: content
        )
    }
}


extension View {
    func toast2<Content>(isPresented: Binding<Bool>, content: @escaping () -> Content) -> some View where Content: View {
        Toast2(
            isPresented: isPresented,
            presenter: { self },
            content: content
        )
    }
}


extension View {
    func toast3<Content>(isPresented: Binding<Bool>, content: @escaping () -> Content) -> some View where Content: View {
        Toast3(
            isPresented: isPresented,
            presenter: { self },
            content: content
        )
    }
}




struct Toast1<Presenting, Content>: View where Presenting: View, Content: View {
    @Binding var isPresented: Bool
    let presenter: () -> Presenting
    let content: () -> Content
    let delay: TimeInterval = 2

    var body: some View {
        if self.isPresented {
            DispatchQueue.main.asyncAfter(deadline: .now() + self.delay) {
                withAnimation {
                    self.isPresented = false
                }
            }
        }

        return GeometryReader { geometry in
            ZStack(alignment: .bottom) {
                self.presenter()
                ZStack {
                    Rectangle()
                        .fill(Color(red: 0.9, green: 0.9, blue: 0.919, opacity: 1.0))
                        .cornerRadius(10)
                        .frame(width: 300, height: 100, alignment: .center)
                    self.content()
                    
                } //ZStack (inner)
                .frame(width: geometry.size.width / 1.25, height: geometry.size.height / 10)
                .opacity(self.isPresented ? 1 : 0)
            } //ZStack (outer)
            .padding(.bottom,10)
            
        } //GeometryReader
    } //body
} //Toast



struct Toast2<Presenting, Content>: View where Presenting: View, Content: View {
    @Environment(\.dismiss) var dismiss
    @Binding var isPresented: Bool
    let presenter: () -> Presenting
    let content: () -> Content
    let delay: TimeInterval = 2
    @EnvironmentObject var reservationFormViewModel:ReservationFormViewModel
    @EnvironmentObject var businessDetailsViewModel:BusinessDetailsViewModel
    @EnvironmentObject var showingSheet:ShowingSheet
//    @EnvironmentObject var businessInfoViewModel:BusinessInfoViewModel

    var body: some View {
        if self.isPresented {
            DispatchQueue.main.asyncAfter(deadline: .now() + self.delay) {
//                withAnimation {
                if(showingSheet.showingSheet){
                    reservationFormViewModel.addReservation(name:businessDetailsViewModel.name,businessID: businessDetailsViewModel.businessID)
                }
                    dismiss()
                print("call add reservation")
//                if(showingSheet.showingSheet){
//                    reservationFormViewModel.addReservation(name:businessDetailsViewModel.name,businessID: businessDetailsViewModel.businessID)
//                }
                    self.isPresented = false
                    
//                }
            }
        }

        return GeometryReader { geometry in
            ZStack(alignment: .bottom) {
                self.presenter()
                ZStack {
                    Rectangle()
                        .fill(Color.green)
                        .cornerRadius(10)
                        .frame(width: 400, height: 1500, alignment: .center)
                    self.content()
                    
                } //ZStack (inner)
                .frame(width: geometry.size.width / 1.25, height: geometry.size.height / 10)
                .opacity(self.isPresented ? 1 : 0)
            } //ZStack (outer)
            .padding(.bottom,10)
            
        } //GeometryReader
    } //body
} //Toast




struct Toast3<Presenting, Content>: View where Presenting: View, Content: View {
    @Binding var isPresented: Bool
    let presenter: () -> Presenting
    let content: () -> Content
    let delay: TimeInterval = 2

    var body: some View {
        if self.isPresented {
            DispatchQueue.main.asyncAfter(deadline: .now() + self.delay) {
                withAnimation {
                    self.isPresented = false
                }
            }
        }

        return GeometryReader { geometry in
            ZStack(alignment: .bottom) {
                self.presenter()
                ZStack {
                    Rectangle()
                        .fill(Color(red: 0.9, green: 0.9, blue: 0.919, opacity: 0.5))
                        .cornerRadius(10)
                        .frame(width: 260, height: 90, alignment: .center)
                    self.content()
                    
                } //ZStack (inner)
                .frame(width: geometry.size.width / 1.25, height: geometry.size.height / 10)
                .opacity(self.isPresented ? 1 : 0)
            } //ZStack (outer)
            .padding(.bottom,15)
        } //GeometryReader
    } //body
} //Toast








//struct Toast_Previews: PreviewProvider {
//    static var previews: some View {
//        Toast()
//    }
//}
