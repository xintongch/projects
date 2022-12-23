//
//  BusinessInfoView.swift
//  yelp-9
//
//  Created by Xintong Chen on 11/25/22.
//

import SwiftUI
import Alamofire
import SwiftyJSON
import MapKit



extension URLCache {
    
    static let imageCache = URLCache(memoryCapacity: 512*1000*1000, diskCapacity: 10*1000*1000*1000)
}



class BusinessInfoViewModel:ObservableObject{
    var businessID:String
    @Published var name:String;
    @Published var address:String=""
    @Published var category:String=""
    @Published var phone:String=""
    @Published var price_range:String=""
    @Published var status:String=""
    @Published var business_link:String=""
    @Published var photos:[String]=[]
    @Published var lat:Double=0
    @Published var lng:Double=0
    @Published var region=MKCoordinateRegion(
        center:CLLocationCoordinate2D(latitude: 0, longitude: 0),
        latitudinalMeters:750,
        longitudinalMeters:750
    )
    @Published var facebook_share_link:String=""
    @Published var twitter_share_link:String=""
    private var twitter_sl="https://twitter.com/intent/tweet?text="
    private var facebook_sl="https://www.facebook.com/sharer/sharer.php?u="
    @Published var mapMarker:IdentifiablePlace=IdentifiablePlace(lat: 0, long: 0)
    //    @Published var mapPin={"name":"0","latitude":0,"longitude":0}
    //    @Published MapPin(coordinate: 0,
    //                      tint: Color.Red)
    private var url="https://yelp-frontend-8-jfb.wl.r.appspot.com/api/detail"
    
    
    
    init(businessID:String,name:String){
        self.name=name
        //        self.region=MKCoordinateRegion(
        //            center:CLLocationCoordinate2D(latitude: self.lat, longitude: self.lng),
        //            latitudinalMeters:750,
        //            longitudinalMeters:750
        //        )
        self.businessID=businessID
        self.url=self.url+"?id="+self.businessID
        //        Task{
        //            await fetchBusinessDetails(self.url)
        //        }
        //        print("businessdetailviewmodelinit")
        
        //        self.category=""
        //        self.phone=""
        //        self.price_range=""
        //        self.status=""
        //        self.business_link=""
        //        self.lat=0
        //        self.lng=0
        //        self.photos=[]
        
        //        print("start fetching info data")
        Task{
            await fetchBusinessDetails(self.url)
        }
        
        
    }
    
    func fetchData(){
        self.category=""
        self.phone=""
        self.price_range=""
        self.status=""
        self.business_link=""
        self.lat=0
        self.lng=0
        self.photos=[]
        
        //        print("start fetching info data")
        Task{
            await fetchBusinessDetails(self.url)
        }
    }
    
    func fetchBusinessDetails(_ url:String)async{
        if let url=URL(string: (url)){
            //            print("inside fetch business detail requesting:\(url)")
            
            try await AF.request(url).validate().responseJSON{
                (response) in
                switch response.result{
                case .success(let JSON):
                    let response=JSON as! NSDictionary
                    //                    print("fetch business detail success")
                    //                    print("response=\(response)")
                    //                    print(response.object(forKey: "location"))
                    //                    print("businessDetailResponse=\(response)")
                    //                    let businessDetails=response.object(forKey: "")
                    if(response.object(forKey: "location") != nil){
                        let location=(response.object(forKey: "location")) as! NSDictionary
                        //                        print("test point:")
                        //                        print(location.object(forKey: "display_address"))
                        if(location.object(forKey: "display_address") != nil){
                            let da=location.object(forKey: "display_address") as! NSArray
                            if(da.count != 0){
                                //                                print("contains key location")
                                self.address=da[0] as! String
                            }
                        }
                    }else{
                        //                        print(response.object(forKey: "location"))
                        //                        print("doesn't contain key location-display_address")
                    }
                    
                    if(response.object(forKey: "categories") != nil){
                        let cat=response.object(forKey: "categories") as! NSArray
                        //                        print("test point cat")
                        //                        print(cat)
                        if(cat.count != 0){
                            //                            print("contain key categories")
                            let c=cat[0] as! NSDictionary
                            self.category=c.object(forKey: "title") as! String
                        }
                    }else{
                        //                        print("doesn't contain key categories")
                    }
                    
                    if(response.object(forKey: "display_phone") != nil){
                        let ph=response.object(forKey: "display_phone") as! String
                        if(ph.count != 0){
                            //                            print("contain key phone")
                            self.phone=ph
                        }
                    }else if(response.object(forKey: "phone") != nil){
                        let ph=response.object(forKey: "phone") as! String
                        if(ph.count != 0){
                            //                            print("contain key phone")
                            self.phone=ph
                        }
                    }else{
                        //                        print("doesn't contain key phone")
                    }
                    
                    if(response.object(forKey: "price") != nil){
                        //                        print("contain key price")
                        let pr=response.object(forKey: "price") as! String
                        self.price_range=pr
                    }else{
                        //                        print("doesn't contain key pr")
                    }
                    
                    if(response.object(forKey: "hours") != nil){
                        let hr=response.object(forKey: "hours") as! NSArray
                        if(hr.count != 0){
                            let h=hr[0] as! NSDictionary
                            if(h.object(forKey: "is_open_now") != nil){
                                //                                print("contain key is_open_now")
                                let is_open=h.object(forKey: "is_open_now") as! Bool
                                if(is_open == false){
                                    self.status="Closed"
                                }else{
                                    self.status="Open Now"
                                }
                            }
                        }
                    }else{
                        //                        print("doesn't contain key hours")
                    }
                    
                    if(response.object(forKey: "url") != nil){
                        let ur=response.object(forKey: "url") as! String
                        if(ur.count != 0){
                            //                            print("contain key url")
                            self.business_link=ur
                            let flink=self.business_link.addingPercentEncoding(withAllowedCharacters: .urlHostAllowed)
                            //                            print("encoded flink=\(flink)")
                            self.facebook_share_link=self.facebook_sl+(flink ?? "")
                            
                            let bl=self.business_link.split(separator: "&")[0]
                            let tlink="Check \(self.name) on Yelp.\n\(bl)".addingPercentEncoding(withAllowedCharacters: .urlHostAllowed)
                            self.twitter_share_link=self.twitter_sl+(tlink ?? "")
                            
                        }
                    }else{
                        //                        print("doesn't contain key url")
                    }
                    
                    if(response.object(forKey: "photos") != nil){
                        let ps=response.object(forKey: "photos") as! NSArray
                        //                        if(p.count != 0){
                        //                            print("contain key photos")
                        //                            p.
                        //                        }
                        if(self.photos.count == 0){
                            ps.forEach{
                                p in
                                self.photos.append(p as! String)
                            }
                        }
                        //                        print("photos=\(ps)")
                    }else{
                        //                        print("doesn't contain key photos")
                    }
                    //                    print("photos=\(self.photos)")
                    
                    if(response.object(forKey: "coordinates") != nil){
                        let co=response.object(forKey: "coordinates") as! NSDictionary
                        if(co.object(forKey: "latitude") != nil){
                            //                            print("contain key lat")
                            self.lat=co.object(forKey: "latitude") as! Double
                        }
                        if(co.object(forKey: "longitude") != nil){
                            //                            print("contain key lng")
                            self.lng=co.object(forKey: "longitude") as! Double
                        }
                    }else{
                        //                        print("doesn't contain key lat lng")
                    }
                    
                    self.region=MKCoordinateRegion(
                        center:CLLocationCoordinate2D(latitude: self.lat, longitude: self.lng),
                        latitudinalMeters:750,
                        longitudinalMeters:750
                    )
                    
                    self.mapMarker=IdentifiablePlace(lat: self.lat, long: self.lng)
                    
                case .failure(_):
                    print("fetch business deail fail")
                }
            }
        }
    }
}


class ShowingSheet:ObservableObject{
    @Published var showingSheet:Bool=false;
}


struct BusinessInfoView:View{
    //    @ObservedObject var businessInfoViewModel:BusinessInfoViewModel
    var iconWidth:CGFloat=50
    var iconHeight:CGFloat=50
    var infoSize:CGFloat=18
    var showToast:Bool=false
    @EnvironmentObject var businessDetailsViewModel:BusinessDetailsViewModel
    @EnvironmentObject var reservationFormViewModel:ReservationFormViewModel
//    @State var showingSheet=false
    var photoWidth:CGFloat=300
    var photoHeight:CGFloat=230
    @StateObject var showingSheet:ShowingSheet=ShowingSheet()
    
    //    @State private var lightsOn: Bool = true
    @State var showToast3: Bool = false
    
    var body: some View{
        VStack{
            
            HStack{
                Text(businessDetailsViewModel.name)
                    .font(.system(size: 26,weight: .bold))
                //                .padding([.top,.bottom],2)
                //                    .background(Color.green)
            }
            
            VStack{
                HStack{
                    VStack(alignment: .leading){
                        HStack{
                            Spacer()
                        }
                        Text("Address")
                            .font(.system(size: infoSize,weight:.bold))
                        Text(businessDetailsViewModel.address)
                            .foregroundColor(.gray)
                            .font(.system(size: infoSize))
                    }
                    VStack(alignment: .trailing){
                        HStack{
                            Spacer()
                        }
                        Text("Category")
                            .font(.system(size: infoSize,weight:.bold))
                        Text(businessDetailsViewModel.category)
                            .foregroundColor(.gray)
                            .font(.system(size: infoSize))
                    }
                }
                
                
                HStack{
                    VStack(alignment: .leading){
                        HStack{
                            Spacer()
                        }
                        Text("Phone")
                            .font(.system(size: infoSize,weight:.bold))
                        Text(businessDetailsViewModel.phone)
                            .foregroundColor(.gray)
                            .font(.system(size: infoSize))
                    }
                    VStack(alignment: .trailing){
                        HStack{
                            Spacer()
                        }
                        Text("Price Range")
                            .font(.system(size: infoSize,weight:.bold))
                        Text(businessDetailsViewModel.price_range)
                            .foregroundColor(.gray)
                            .font(.system(size: infoSize))
                    }
                }
                
                
                HStack{
                    VStack(alignment: .leading){
                        HStack{
                            Spacer()
                        }
                        Text("Status")
                            .font(.system(size: infoSize,weight:.bold))
                        if(businessDetailsViewModel.status == "Closed"){
                            Text(businessDetailsViewModel.status)
                                .foregroundColor(.red)
                                .font(.system(size: infoSize))
                        }else{
                            Text(businessDetailsViewModel.status)
                                .foregroundColor(.green)
                                .font(.system(size: infoSize))
                        }
                    }
                    VStack(alignment: .trailing){
                        HStack{
                            Spacer()
                        }
                        Text("Visit Yelp for more")
                            .font(.system(size: infoSize,weight:.bold))
                        Text(.init("[Business Link](\(businessDetailsViewModel.business_link))"))
                            .font(.system(size: infoSize))
                    }
                }
            }
            .padding(.top,0)
            
            
            HStack{
                if reservationFormViewModel.checkForKey(businessID: businessDetailsViewModel.businessID) == false{
                    Button(action: {
                        showingSheet.showingSheet=true
                    }){
                        Text("Reserve Now")
                    }
                    .foregroundColor(Color.white)
                    .padding()
                    .background(Color.red)
                    .cornerRadius(15)
                    .sheet(isPresented: $showingSheet.showingSheet,content: {
                        ReservationFormView()
                    })
                }
                
                
                if reservationFormViewModel.checkForKey(businessID: businessDetailsViewModel.businessID) == true{
                    Button("Cancel Reservation"){
                        reservationFormViewModel.cancelReservation(businessID: businessDetailsViewModel.businessID)
                        self.showToast3=true
                    }
                    .padding()
                    .background(Color.blue)
                    .cornerRadius(15)
                    .foregroundColor(Color.white)
                }
            }
            .padding(.top,30)
            
            HStack{
                Text("Share on:")
                    .font(.system(size: 18,weight:.semibold))
                Button(action: {
                    guard let google = URL(string: "\(businessDetailsViewModel.facebook_share_link)"),
                          UIApplication.shared.canOpenURL(google) else {
                        return
                    }
                    UIApplication.shared.open(google,
                                              options: [:],
                                              completionHandler: nil)
                }) {
                    Image("facebook_logo")
                        .resizable()
                        .frame(width: iconWidth,
                               height: iconHeight)
                        .buttonStyle(PlainButtonStyle())
//                        .opacity(0.1)
//                        .onTapGesture {
//                            .opacity(0.1)
//                        }
                }
                
                
                
                Button(action: {
                    guard let google = URL(string: "\(businessDetailsViewModel.twitter_share_link)"),
                          UIApplication.shared.canOpenURL(google) else {
                        return
                    }
                    UIApplication.shared.open(google,
                                              options: [:],
                                              completionHandler: nil)
                }) {
                    Image("twitter_logo")
                        .resizable()
                        .frame(width: iconWidth,
                               height: iconHeight)
                        .buttonStyle(PlainButtonStyle())
                }
                
                
                
                
            }
            .padding(.top,10)
            
            
            if(businessDetailsViewModel.photos.count != 0){
                VStack(spacing:0){
                    TabView{
                        ForEach(businessDetailsViewModel.photos,id:\.self){
                            photo in
                            //                            LazyImage(source: "https://picsum.photos/200/300",content:{
                            
                            //                            })
                            CachedAsyncImage(urlRequest: URLRequest(url: URL(string: photo)!), urlCache: .imageCache,content:{
                                //                            })
                                //                            AsyncImage(url: URL(string: photo),content: {
                                image in
                                image
                                    .resizable()
                                    .scaledToFill()
                                    .frame(width: photoWidth, height: photoHeight)
                                    .cornerRadius(5)
                                //                                            .aspectRatio(contentMode: .fit)
                            }, placeholder: {
                                ProgressView()
                            })
                            .frame(width: photoWidth, height: photoHeight)
                            .padding(.horizontal,15)
                        }
                        .padding(.top,0)
                    }
                    .tabViewStyle(PageTabViewStyle(indexDisplayMode: .always))
                    .padding(.vertical,20)
                }
                .padding(.top,-10)
                Spacer()
            }
        }
        .padding([.leading,.trailing],15)
        .padding([.top],-40)
        .environmentObject(businessDetailsViewModel)
        .environmentObject(reservationFormViewModel)
        .environmentObject(showingSheet)
        .toast3(isPresented: self.$showToast3) {
            HStack {
                Text("Your reservation is cancelled")
            } //HStack
        } //toast
        //        .onAppear{
        //            businessInfoViewModel.fetchData()
        //        }
        //        Text("info")
        
        
    }
    
    
    
    
    
}





struct ReserveButtonView:View{
    @State private var showingSheet=false
    @State var showToast3: Bool = false
    @EnvironmentObject var reservationFormViewModel:ReservationFormViewModel
    //    @EnvironmentObject var businessInfoViewModel:BusinessInfoViewModel
    @EnvironmentObject var businessDetailsViewModel:BusinessDetailsViewModel
    
    var body: some View{
        
        VStack{
            if reservationFormViewModel.checkForKey(businessID: businessDetailsViewModel.businessID) == false{
                Button(action: {
                    showingSheet=true
                }){
                    Text("Submit")
                }
                .foregroundColor(Color.white)
                .padding()
                .background(Color.red)
                .cornerRadius(15)
                .sheet(isPresented: $showingSheet,content: {
                    ReservationFormView()
                })
            }
            
            if reservationFormViewModel.checkForKey(businessID: businessDetailsViewModel.businessID) == true{
                Button("Cancel Reservation"){
                    reservationFormViewModel.cancelReservation(businessID: businessDetailsViewModel.businessID)
                    self.showToast3=true
                    //                    businessInfoViewModel.setFalse()
                }
                .padding()
                .background(Color.blue)
                .cornerRadius(15)
                .foregroundColor(Color.white)
            }
            
        }
        .toast3(isPresented: self.$showToast3) {
            HStack {
                Text("Your reservation is cancelled")
            } //HStack
        } //toast
        
    }
}


//class BusinessInfoViewModel:ObservableObject{
//    @Published var reserved:Int=0;
//    init(){
//        print("businessInfoView init!!!!!")
//    }
//    func setFalse(){
//        print("set false")
//        self.reserved=0;
//    }
//    func setTrue(){
//        print("set true")
//        self.reserved=1;
//    }
//}




//struct BusinessInfoView_Previews: PreviewProvider {
//    static var previews: some View {
//        BusinessInfoView()
//            .background(Color.gray)
//    }
//}
