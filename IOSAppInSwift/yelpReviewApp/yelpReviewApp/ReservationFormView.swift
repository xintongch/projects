//
//  ReservationFormView.swift
//  yelp-9
//
//  Created by Xintong Chen on 11/25/22.
//

import SwiftUI
//import Alamofire
//import SwiftyJSON
import MapKit

//import Kingfisher


import Combine
class TextValidator: ObservableObject {
    @Published var text = ""
}

struct ReservationFormView:View{
    
    private var hourOptions:[String]=["10","11","12","13","14","15","16","17"]
    private var minuteOptions:[String]=["00","15","30","45"]
    
    let delay: TimeInterval = 2
    @ObservedObject var textValidator = TextValidator()
    
    @State private var lightsOn: Bool = true
    @State private var showToast1: Bool = false
    @State private var showToast2: Bool = false
    
    @Environment(\.dismiss) var dismiss
    @EnvironmentObject var businessDetailsViewModel:BusinessDetailsViewModel
    @EnvironmentObject var reservationFormViewModel:ReservationFormViewModel
    @EnvironmentObject var showingSheet:ShowingSheet
    @State private var showText=false
    var body: some View{
        
        VStack{
//            Text("Reservation Form")
            Form{
                Section{
                            Text("Reservation Form")
                                .font(.system(size: 26,weight:.bold))
                                .frame(maxWidth: .infinity, alignment: .center)
                }
                Section{
                        Text(businessDetailsViewModel.name)
                            .font(.system(size: 26,weight:.bold))
                            .frame(maxWidth: .infinity, alignment: .center)
                }
                Section{
                    HStack{
                        Text("Email:")
                            .foregroundColor(.gray)
                            .frame(height: 55)
                        TextField("", text: $reservationFormViewModel.email)
                    }
                    
                    HStack{
                        DatePicker("Date/Time:",selection: $reservationFormViewModel.datee,in:Date()...,displayedComponents: .date)
                            .foregroundColor(.gray)
//                            .environment(\.locale, Locale.init(identifier: "en_gb"))
                        
                        HStack{
                            Picker(
                                selection:$reservationFormViewModel.hour,
                                label:
                                    HStack{
                                        Text("filter:")
                                        Text(reservationFormViewModel.hour)
                                    },
                                content: {
                                    ForEach(hourOptions,id:\.self){
                                        option in
                                        Text(option)
                                            .foregroundColor(.black)
                                            .tag(option)
                                    }
                                }
                            )
                            .pickerStyle(MenuPickerStyle())
                            .accentColor(.black)
                            
                            Text(":")
                            
                            Picker(
                                selection:$reservationFormViewModel.minute,
                                label:
                                    HStack{
                                        Text("filter:")
                                        Text(reservationFormViewModel.minute)
                                    },
                                content: {
                                    ForEach(minuteOptions,id:\.self){
                                        option in
                                        Text(option)
                                            .tag(option)
                                    }
                                }
                            )
                            .pickerStyle(MenuPickerStyle())
                            .accentColor(.black)
                        }
                        .background(Color(red: 0.949, green: 0.949, blue: 0.97, opacity: 1.0))
                        .cornerRadius(5)
                    }
                    .frame(height: 65)
                    
                    
                    //                    DatePicker("Date/Time",selection: $reservationFormViewModel.datee,in:Date()...)
                    VStack(alignment: .center){
                        Button(action: {
                            if(reservationFormViewModel.email == ""){
                                self.showToast1=true
                            }else{
                                let emailRegEx = "[A-Z0-9a-z._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,64}"
                                let emailPred = NSPredicate(format:"SELF MATCHES %@", emailRegEx)
                                let result = emailPred.evaluate(with: reservationFormViewModel.email)
                                if(result == false){
                                    self.showToast1=true
                                }else{
                                    
                                    //                                                                reservationFormViewModel.addReservation(name:businessDetailsViewModel.name,businessID: businessDetailsViewModel.businessID)
                                    self.showToast2=true
                                }
                            }
                        }){
                            Text("Submit")
                                .frame(width: 120, height: 50, alignment: .center)
                                .font(.system(size: 18))
                        }
                        .foregroundColor(Color.white)
                        .background(Color.blue)
                        .cornerRadius(15)
                        .buttonStyle(BorderlessButtonStyle())
                        .frame(width: .infinity, height: 80)
                        .padding(.leading,100)
                    }
                }
            }
            .toast1(isPresented: self.$showToast1) {
                HStack {
                    Text("Please enter a valid email")
                } //HStack
            } //toast
            
            .toast2(isPresented: self.$showToast2) {
                VStack {
                    VStack{
                        Text("Congratulations!")
                            .font(.system(size: 16,weight: .bold))
                        Text("You have successfully made an reservation at")
                            .padding(.top,20)
                        Text(businessDetailsViewModel.name)
                    }
                    .padding(.top,-400)
                    .foregroundColor(.white)
                    //                    Spacer()
                    Button(action: {
                        dismiss()
                        reservationFormViewModel.addReservation(name:businessDetailsViewModel.name,businessID: businessDetailsViewModel.businessID)
                        showingSheet.showingSheet=false
                        //                        self.isPresented = false
                    }){
                        Text("Done")
                            .frame(width: 200, height: 50, alignment: .center)
                            .font(.system(size: 18))
                    }
                    .foregroundColor(Color.green)
                    .background(Color.white)
                    .cornerRadius(30)
                    .buttonStyle(BorderlessButtonStyle())
                    .padding(.top,115)
                } //HStack
            } //toast
            Spacer()
        }
        .background(Color(red: 0.949, green: 0.949, blue: 0.97, opacity: 1.0))
        .environmentObject(reservationFormViewModel)
        .environmentObject(businessDetailsViewModel)
        .environmentObject(showingSheet)
    }
    
    
    
    private func delayText() async {
        // Delay of 7.5 seconds (1 second = 1_000_000_000 nanoseconds)
        try? await Task.sleep(nanoseconds: 7_500_000_000)
        //            hasTimeElapsed = true
    }
}







struct Reservation:Hashable,Encodable,Decodable{
    let name:String
    let businessID:String
    let email:String;
    let date:String
    let hour:String
    let minute:String
    init(name:String, businessID:String, email:String,date:String,hour:String,minute:String){
        self.name=name
        self.businessID=businessID
        self.email=email
        self.date=date
        self.hour=hour
        self.minute=minute
    }
}

//struct ReservationIndexMap{
//    let businessID:String
//    let index:Int
//    init(businessID:String,index:Int){
//        self.businessID=businessID
//        self.index=index
//    }
//}


//struct ReservationDictionary:RandomAccessCollection{
//    let ID:String
//    let reservation:Reservation
//    init(ID:String,)
//}


class ReservationFormViewModel:ObservableObject{
    @Published var email:String=""
    @Published var datee:Date=Date()
    @Published var date:String=""
    @Published var hour:String="10"
    @Published var minute:String="00"
    @Published var reservations:[Reservation]=[]
    @Published var reservationIndexMap:[String:Int]=[:]
    @Published var dismissed:Bool=false
    private var index=1;
    
    init(){
        //        UserDefaults.standard.removeObject(forKey: "reservations")
        //        UserDefaults.standard.removeObject(forKey: "reservationIndexMap")
        //        UserDefaults.standard.removeObject(forKey: "index")
        
        print("reservationformviewmodel init")
        self.index=1
        if(UserDefaults.standard.object(forKey:"reservations") == nil){
            if let encoded = try? JSONEncoder().encode(self.reservations) {
                UserDefaults.standard.set(encoded,forKey: "reservations")
            }
        }else{
            if let data = UserDefaults.standard.object(forKey: "reservations") as? Data,
               let category = try? JSONDecoder().decode([Reservation].self, from: data) {
                self.reservations=category
            }
        }
        
        
        if(UserDefaults.standard.string(forKey:"index") == nil){
            //            if let encoded = try? JSONEncoder().encode(self.reservations) {
            UserDefaults.standard.set(self.index,forKey: "index")
            //            }
        }else{
            self.index=Int(UserDefaults.standard.string(forKey: "index") as! String) ?? 1
            //            if let data = UserDefaults.standard.object(forKey: "reservations") as? Data,
            //               let category = try? JSONDecoder().decode(Reservation.self, from: data) {
            //            }
        }
        
        
        if(UserDefaults.standard.object(forKey:"reservationIndexMap") == nil){
            if let encoded2 = try? JSONEncoder().encode(self.reservationIndexMap) {
                UserDefaults.standard.set(encoded2,forKey: "reservationIndexMap")
            }
        }else{
            if let data2 = UserDefaults.standard.object(forKey: "reservationIndexMap") as? Data,
               let cate = try? JSONDecoder().decode([String:Int].self, from: data2) {
                self.reservationIndexMap=cate
            }
        }
        
        
        
        
        
    }
    func addReservation(name:String,businessID:String){
        if self.checkForKey(businessID: businessID){
            return
        }
        let da="\(self.datee)"
        let datetime=da.components(separatedBy: " ")
        print("before adding:\(self.reservations)")
        let time="\(self.hour):\(self.minute)"
        self.reservations.append(Reservation(name:name,businessID:businessID, email: email, date: datetime[0], hour: hour, minute: minute))
        print("after adding:\(self.reservations)")
        self.addInRImap(businessID:businessID);
        
        self.email=""
        self.datee=Date()
        self.hour="10"
        self.minute="00"
        
        
        if let encoded = try? JSONEncoder().encode(self.reservations) {
            UserDefaults.standard.set(encoded,forKey: "reservations")
        }
        if let encoded2 = try? JSONEncoder().encode(self.reservationIndexMap) {
            UserDefaults.standard.set(encoded2,forKey: "reservationIndexMap")
        }
        UserDefaults.standard.set(self.index,forKey: "index")
        
        print("after adding:\(self.reservations)")
    }
    
    func cancelReservation(businessID:String){
        print("before remove:\(self.reservations)")
        //        self.reservations.removeValue(forKey: businessID)
        let pos=self.reservationIndexMap[businessID] as! Int
        self.removeInRImap(businessID: businessID)
        self.reservations.remove(at:pos-1)
        
        print("after remove:\(self.reservations)")
        
        
        if let encoded = try? JSONEncoder().encode(self.reservations) {
            UserDefaults.standard.set(encoded,forKey: "reservations")
        }
        if let encoded2 = try? JSONEncoder().encode(self.reservationIndexMap) {
            UserDefaults.standard.set(encoded2,forKey: "reservationIndexMap")
        }
        UserDefaults.standard.set(self.index,forKey: "index")
        
    }
    
    func addInRImap(businessID:String){
        print("before index:\(self.reservationIndexMap)")
        self.reservationIndexMap[businessID]=index;
        index=index+1;
        print("after index:\(self.reservationIndexMap)")
    }
    
    func removeInRImap(businessID:String){
        print("before remove index:\(self.reservationIndexMap)")
        let pos=self.reservationIndexMap[businessID] as! Int
        let len=self.reservations.count as! Int
        if(pos<=len-1){
            for i in pos...len-1{
                let bID=self.reservations[i].businessID as! String
                self.reservationIndexMap[bID]=i
            }
        }
        self.reservationIndexMap[businessID]=nil
        self.index=self.index-1
        print("after remove index:\(self.reservationIndexMap)")
    }
    
    func checkForKey(businessID:String) -> Bool{
        if(self.reservationIndexMap[businessID] != nil){
            return true;
        }else{
            return false
        }
    }
    
    func checkForBusinessID(index:Int)->String{
        return self.reservations[index].businessID
    }
}



struct ReservationFormView_Previews: PreviewProvider {
    static var previews: some View {
        ReservationFormView()
    }
}
