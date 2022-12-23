//
//  BookingsView.swift
//  yelp-9
//
//  Created by Xintong Chen on 11/25/22.
//

import SwiftUI
//import Alamofire
//import SwiftyJSON
import MapKit

//class ReservationFormViewModel:ObservableObject{
////    @Published var email:String=""
////    @Published var date:String=""
////    @Published var time:String=""
//    @Published var reservations:[Reservation]=[]
//    init(){
//        print("reservation form init")
//    }
//    func addReservation(email:String,date:String,time:String){
//        print("before adding:\(self.reservations)")
//        self.reservations.append(Reservation(email: email, date: date, time: time))
//        print("after adding:\(self.reservations)")
//    }
//}

struct BookingsView:View{
    private var bookingSize:CGFloat=12
    @EnvironmentObject var reservationFormViewModel:ReservationFormViewModel
    var body: some View{
        //        Text("bookings")
                if(reservationFormViewModel.reservations.count == 0){
                    Text("No bookings found")
                    //                .padding(.top,0)
                    //                .font(.system(size: 36,weight: .bold))
                        .foregroundColor(.red)
                }else{
        
//        NavigationView{
            List{
                ForEach(reservationFormViewModel.reservations,id:\.self){
                    reservation in
                    HStack{
                        Text(reservation.name)
                            .frame(width: 60, alignment: .center)
                            .font(.system(size: bookingSize))
                        Text(reservation.date)
                            .frame(width: 80, alignment: .center)
                            .font(.system(size: bookingSize))
                        Text("\(reservation.hour):\(reservation.minute)")
                            .frame(width: 50, alignment: .center)
                            .font(.system(size: bookingSize))

                        Text(reservation.email)
                            .frame(width: 100, alignment: .center)
                            .font(.system(size: bookingSize))
                    }
                }
                .onDelete{
                    indexSet in
                    for ind in indexSet{
                        let businessID=reservationFormViewModel.checkForBusinessID(index: ind)
                        reservationFormViewModel.cancelReservation(businessID: businessID)
                    }
                }
            }
            .navigationTitle("Your Reservations")
//        }
//        .padding(.top,-250)
        
                }
        
//        Text("No bookings found")
////                        .padding(.top,-100)
//        //                .font(.system(size: 36,weight: .bold))
//            .foregroundColor(.red)
        
        
        
        //        }
    }
}

//struct BookingsView_Previews: PreviewProvider {
//    static var previews: some View {
//        BookingsView()
//    }
//}
