//
//  BusinessMapLocationView.swift
//  yelp-9
//
//  Created by Xintong Chen on 11/25/22.
//

import SwiftUI
import Alamofire
import SwiftyJSON
import MapKit

struct BusinessMapLocationView:View{
//    @ObservedObject var businessMapViewModel:BusinessMapViewModel
    @EnvironmentObject var businessDetailsViewModel:BusinessDetailsViewModel
    //    @State private var region=MKCoordinateRegion(
    //        center:CLLocationCoordinate2D(latitude: businessDetailsViewModel.lat, longitude: businessDetailsViewModel.lng),
    //        latitudinalMeters:750,
    //        longitudinalMeters:750
    //    )
    
    var body: some View{
        //        Text("business map")
        Map(coordinateRegion:$businessDetailsViewModel.region,annotationItems: [businessDetailsViewModel.mapMarker])
        { place in
            MapMarker(coordinate: place.location,
                      tint: Color.red)
        }
//        .onAppear{
//            businessDetailsViewModel.fetchData()
//        }
    }
}




//class BusinessMapViewModel:ObservableObject{
//    @Published var businessID:String
//    @Published var name:String;
//    @Published var address:String="test address"
//    @Published var category:String=""
//    @Published var phone:String=""
//    @Published var price_range:String=""
//    @Published var status:String=""
//    @Published var business_link:String=""
//    @Published var photos:[String]=[]
//    @Published var lat:Double=0
//    @Published var lng:Double=0
////    private var span= MKCoordinateSpan(latitudeDelta: 0.5, longitudeDelta: 0.5)
////    @Published var region=MKCoordinateRegion(
////        center:CLLocationCoordinate2D(latitude: 0, longitude: 0),
////        latitudinalMeters:1500,
////        longitudinalMeters:1500
////    )
//    @Published var region=MKCoordinateRegion(
//        center:CLLocationCoordinate2D(latitude: 0, longitude: 0),
//        span:MKCoordinateSpan(latitudeDelta: 0.01, longitudeDelta: 0.01)
//    )
//    @Published var facebook_share_link:String=""
//    @Published var twitter_share_link:String=""
//    private var twitter_sl="https://twitter.com/intent/tweet?text="
//    private var facebook_sl="https://www.facebook.com/sharer/sharer.php?u="
//    @Published var mapMarker:IdentifiablePlace=IdentifiablePlace(lat: 0, long: 0)
//    //    @Published var mapPin={"name":"0","latitude":0,"longitude":0}
//    //    @Published MapPin(coordinate: 0,
//    //                      tint: Color.Red)
//    private var url="https://yelp-frontend-8-jfb.wl.r.appspot.com/api/detail"
//    init(businessID:String,name:String){
//        self.name=name
//        //        self.region=MKCoordinateRegion(
//        //            center:CLLocationCoordinate2D(latitude: self.lat, longitude: self.lng),
//        //            latitudinalMeters:750,
//        //            longitudinalMeters:750
//        //        )
//        self.businessID=businessID
//        self.url=self.url+"?id="+self.businessID
//        //        Task{
//        //            await fetchBusinessDetails(self.url)
//        //        }
////        print("businessdetailviewmodelinit")
//    }
//
//    func fetchData(){
//        self.category=""
//        self.phone=""
//        self.price_range=""
//        self.status=""
//        self.business_link=""
//        self.lat=0
//        self.lng=0
//        self.photos=[]
//
////        print("start fetching data")
//        Task{
//            await fetchBusinessDetails(self.url)
//        }
//    }
//
//    func fetchBusinessDetails(_ url:String)async{
//        if let url=URL(string: (url)){
////            print("inside fetch business detail requesting:\(url)")
//
//            try await Alamofire.request(url).validate().responseJSON{
//                (response) in
//                switch response.result{
//                case .success(let JSON):
//                    let response=JSON as! NSDictionary
////                    print("fetch business detail success")
////                    print("response=\(response)")
//                    //                    print(response.object(forKey: "location"))
//                    //                    print("businessDetailResponse=\(response)")
//                    //                    let businessDetails=response.object(forKey: "")
//                    if(response.object(forKey: "location") != nil){
//                        let location=(response.object(forKey: "location")) as! NSDictionary
////                        print("test point:")
////                        print(location.object(forKey: "display_address"))
//                        if(location.object(forKey: "display_address") != nil){
//                            let da=location.object(forKey: "display_address") as! NSArray
//                            if(da.count != 0){
////                                print("contains key location")
//                                self.address=da[0] as! String
//                            }
//                        }
//                    }else{
//                        //                        print(response.object(forKey: "location"))
////                        print("doesn't contain key location-display_address")
//                    }
//
//                    if(response.object(forKey: "categories") != nil){
//                        let cat=response.object(forKey: "categories") as! NSArray
////                        print("test point cat")
////                        print(cat)
//                        if(cat.count != 0){
////                            print("contain key categories")
//                            let c=cat[0] as! NSDictionary
//                            self.category=c.object(forKey: "title") as! String
//                        }
//                    }else{
////                        print("doesn't contain key categories")
//                    }
//
//                    if(response.object(forKey: "display_phone") != nil){
//                        let ph=response.object(forKey: "display_phone") as! String
//                        if(ph.count != 0){
////                            print("contain key phone")
//                            self.phone=ph
//                        }
//                    }else if(response.object(forKey: "phone") != nil){
//                        let ph=response.object(forKey: "phone") as! String
//                        if(ph.count != 0){
////                            print("contain key phone")
//                            self.phone=ph
//                        }
//                    }else{
////                        print("doesn't contain key phone")
//                    }
//
//                    if(response.object(forKey: "price") != nil){
////                        print("contain key price")
//                        let pr=response.object(forKey: "price") as! String
//                        self.price_range=pr
//                    }else{
////                        print("doesn't contain key pr")
//                    }
//
//                    if(response.object(forKey: "hours") != nil){
//                        let hr=response.object(forKey: "hours") as! NSArray
//                        if(hr.count != 0){
//                            let h=hr[0] as! NSDictionary
//                            if(h.object(forKey: "is_open_now") != nil){
////                                print("contain key is_open_now")
//                                let is_open=h.object(forKey: "is_open_now") as! Bool
//                                if(is_open == false){
//                                    self.status="Closed"
//                                }else{
//                                    self.status="Open Now"
//                                }
//                            }
//                        }
//                    }else{
////                        print("doesn't contain key hours")
//                    }
//
//                    if(response.object(forKey: "url") != nil){
//                        let ur=response.object(forKey: "url") as! String
//                        if(ur.count != 0){
////                            print("contain key url")
//                            self.business_link=ur
//                            let flink=self.business_link.addingPercentEncoding(withAllowedCharacters: .urlHostAllowed)
////                            print("encoded flink=\(flink)")
//                            self.facebook_share_link=self.facebook_sl+(flink ?? "")
//
//                            let bl=self.business_link.split(separator: "&")[0]
//                            let tlink="Check \(self.name) on Yelp.\n\(bl)".addingPercentEncoding(withAllowedCharacters: .urlHostAllowed)
//                            self.twitter_share_link=self.twitter_sl+(tlink ?? "")
//
//                        }
//                    }else{
////                        print("doesn't contain key url")
//                    }
//
//                    if(response.object(forKey: "photos") != nil){
//                        let ps=response.object(forKey: "photos") as! NSArray
//                        //                        if(p.count != 0){
//                        //                            print("contain key photos")
//                        //                            p.
//                        //                        }
//                        ps.forEach{
//                            p in
//                            self.photos.append(p as! String)
//                        }
////                        print("photos=\(ps)")
//                    }else{
////                        print("doesn't contain key photos")
//                    }
////                    print("photos=\(self.photos)")
//
//                    if(response.object(forKey: "coordinates") != nil){
//                        let co=response.object(forKey: "coordinates") as! NSDictionary
//                        if(co.object(forKey: "latitude") != nil){
////                            print("contain key lat")
////                            self.lat=co.object(forKey: "latitude") as! Double
//                        }
//                        if(co.object(forKey: "longitude") != nil){
////                            print("contain key lng")
////                            self.lng=co.object(forKey: "longitude") as! Double
//                        }
//                    }else{
////                        print("doesn't contain key lat lng")
//                    }
//
////                    self.region=MKCoordinateRegion(
////                        center:CLLocationCoordinate2D(latitude: self.lat, longitude: self.lng),
////                        latitudinalMeters:55,
////                        longitudinalMeters:55
////                    )
//
////                    self.region=MKCoordinateRegion(
////                        center:CLLocationCoordinate2D(latitude: self.lat, longitude: self.lng),
////                        span:MKCoordinateSpan(latitudeDelta: 10, longitudeDelta: 10)
////                    )
//
//
////                    MKCoordinateRegion(center: CLLocationCoordinate2D(latitude: 51.507222, longitude: -0.1275), span: MKCoordinateSpan(latitudeDelta: 0.5, longitudeDelta: 0.5))
//
////                    self.mapMarker=IdentifiablePlace(lat: self.lat, long: self.lng)
//
//                case .failure(_):
//                    print("fetch business deail fail")
//                }
//            }
//        }
//    }
//}
//
//
//
//
//
//
//
//

//struct BusinessMapLocationView_Previews: PreviewProvider {
//    static var previews: some View {
//        BusinessMapLocationView()
//    }
//}
