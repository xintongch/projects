//
//  ContentView.swift
//  yelp-9
//
//  Created by Xintong Chen on 11/24/22.
//

//import Kingfisher
import SwiftUI
import Alamofire
import SwiftyJSON
import MapKit
import UIKit
//import class Kingfisher.KingfisherManager
//import class Kingfisher.KFImage

//import KingfisherSwiftUI
//import class Kingfisher.framework









enum Flavor: String, CaseIterable, Identifiable {
    case chocolate, vanilla, strawberry
    var id: Self { self }
}


//class TextValidator:ObservableObject{
//    @Published var validated:Bool=false;
//    func validateText(_ keyword:String,_ distance:String,_ location:String,_ auto_detect:Bool)->Bool{
//        if(keyword==""){
//            return false
//        }
//        if(distance==""){
//            return false
//        }
//        if(auto_detect==false && location==""){
//            return false
//        }
//        return true
//    }
//}


//struct AlwaysPopoverModifier<PopoverContent>: ViewModifier where PopoverContent: View {
//
//    let isPresented: Binding<Bool>
//    let contentBlock: () -> PopoverContent
//
//    private func presentPopover() {
//        let contentController = ContentViewController(rootView: contentBlock(), isPresented: isPresented)
//        contentController.modalPresentationStyle = .popover
//
//        let view = store.anchorView
//        guard let popover = contentController.popoverPresentationController else { return }
//        popover.sourceView = view
//        popover.sourceRect = view.bounds
//        popover.delegate = contentController
//
//        guard let sourceVC = view.closestVC() else { return }
//        if let presentedVC = sourceVC.presentedViewController {
//            presentedVC.dismiss(animated: true) {
//                sourceVC.present(contentController, animated: true)
//            }
//        } else {
//            sourceVC.present(contentController, animated: true)
//        }
//    }
//}




struct ContentView: View {
    private var buttonWidth:CGFloat=120
    private var buttonHeight:CGFloat=50
    private var buttonPaddingX:CGFloat=15
    private var buttonPaddingY:CGFloat=20
    private var resultListHeight:CGFloat=50
    @State var showsPopover = false
    @State var showsAlwaysPopover = true
    
    @ObservedObject var formViewModel:FormViewModel=FormViewModel()
    //    @State private var keyword:String=""
    //    @State private var distance:String=""
    //    @State private var auto_detect=false
    //    @State private var location:String=""
    //    @State private var selectedCate:String=""
    @State private var submitEnterKey:Bool=false
    @ObservedObject var reservationFormViewModel:ReservationFormViewModel=ReservationFormViewModel()
    @State private var selectedFlavor: Flavor = .chocolate
    //    @ObservedObject var textValidator:TextValidator=TextValidator()
    //    private var validated:Bool=false;
    
    //    @State private var results;
    @ObservedObject var autoCompleteModel:AutoCompleteModel=AutoCompleteModel()
    let strings=["h","j"]
    var d:Bool=false
    @State var isShowingPopover = true
    private var showText=true
    //    @State var selection:String="Default"
    let filterOptions:[String]=["Default","Arts and Entertainment","Health and Medical",
                                "Hotel and Travel","Food","Professional Services"]
    //    let optionTagMap:[String:String]=["Default":"all","Arts and Entertainment":"arts","Health and Medical":"health",
    //                                      "Hotel and Travel":"hotelstravel","Food":"food","Professional Services":"professional"]
    var body: some View {
        
        NavigationView{
            VStack{
                Form{
                    Section{
                        HStack{
                            
                            Text("Keyword: ")
                                .foregroundColor(Color.gray)
                            
                            TextField("Keyword",text:$formViewModel.keyword,prompt: Text("Required"))
                                .onSubmit {
                                    //                                    print("sumbit enter key")
                                    self.submitEnterKey=true
                                    autoCompleteModel.getAutoComplete(formViewModel.keyword)
                                }
                                .alwaysPopover(isPresented: $submitEnterKey) {
                                    if(autoCompleteModel.fetching == true){
                                        
                                        ProgressView()
                                        { Text("loading...") }
                                        //                                .background(Color.red)
                                            .frame(maxWidth: .infinity, alignment: .center)
                                            .padding([.bottom],15)
                                            .padding([.top],15)
                                        
                                        
                                    }else{
                                        VStack{
                                            ForEach(autoCompleteModel.autoCompleteList,id:\.self){
                                                ac in
                                                Text(ac)
                                                    .padding([.trailing], 15)
                                                    .padding([.leading], 15)
                                                //                                                .padding([.bottom],-20)
                                                //                                                .padding([.top],-20)
                                                    .foregroundColor(Color.gray)
                                                    .onTapGesture {
                                                        formViewModel.keyword=ac
                                                        self.submitEnterKey=false
                                                    }
                                            }
                                        }
                                        
                                        .padding([.bottom],20)
                                        .padding([.top],15)
                                        
                                    }
                                    
                                    //                                            List(autoCompleteModel.autoCompleteList,id:\.self){
                                    //                                                ac in
                                    //                                                Text(ac)
                                    //                                            }
                                    //                                    autoCompleteModel.autoCompleteList.forEach{
                                    //                                        ac in
                                    //                                        Text(ac)
                                    //                                    }
                                    //                                    Text("submit enter key")
                                    //                                    AutoCompleteView()
                                    //                                        .environmentObject(autoCompleteModel)
                                }
                            //                                .environmentObject(autoCompleteModel)
                            
                            //                                .popover(isPresented: $isShowingPopover,content: {
                            //                                    Text("Popr Content")
                            //                                        .padding()
                            //                                        .frame(width: 200, height: 200)
                            //                                })
                            
                            //                            {
                            
                            //                                        }
                        }
                        //                        .environmentObject(autoCompleteModel)
                        HStack{
                            Text("Distance: ")
                                .foregroundColor(Color.gray)
                            TextField("Distance",text:$formViewModel.distance)
                        }
                        HStack{
                            Text("Category: ")
                                .foregroundColor(Color.gray)
                            
                            Picker(
                                selection:$formViewModel.selection,
                                label:
                                    HStack{
                                        Text("filter:")
                                        Text(formViewModel.selection)
                                    },
                                content: {
                                    ForEach(filterOptions,id:\.self){
                                        option in
                                        Text(option)
                                            .tag(option)
                                    }
                                }
                            )
                            .pickerStyle(MenuPickerStyle())
                            
                        }
                        
                        if(formViewModel.auto_detect==false){
                            HStack{
                                Text("Location: ")
                                    .foregroundColor(Color.gray)
                                TextField("Location",text: $formViewModel.locaiton,prompt: Text("Required"))
                            }
                        }
                        
                        Toggle("Auto-detect my location",isOn: $formViewModel.auto_detect).toggleStyle(SwitchToggleStyle(tint:.green))
                        
                        HStack{
                            HStack{
                                if(formViewModel.validateText()==true){
                                    Button(action: {
                                        formViewModel.submit()
                                    }){
                                        Text("Submit")
                                            .frame(width: buttonWidth, height: buttonHeight, alignment: .center)
                                            .font(.system(size: 18))
                                    }
                                    .foregroundColor(Color.white)
                                    
                                    .background(Color.red)
                                    .cornerRadius(10)
                                    .buttonStyle(BorderlessButtonStyle())
                                    //                                .frame(width: 150, height: 90, alignment: .center)
                                    
                                }else{
                                    Button(action: {
                                        formViewModel.submit()
                                    }){
                                        Text("Submit")
                                            .frame(width: buttonWidth, height: buttonHeight, alignment: .center)
                                            .font(.system(size: 18))
                                        
                                    }
                                    .foregroundColor(Color.white)
                                    
                                    .background(Color.gray)
                                    .cornerRadius(10)
                                    .buttonStyle(BorderlessButtonStyle())
                                    //                                .frame(width: 160, height: 90, alignment: .center)
                                    .disabled(true)
                                }
                            }
                            .padding([.leading, .trailing], buttonPaddingX)
                            .padding([.bottom, .top], buttonPaddingY)
                            
                            //                            SubmitButtonView(keyword: keyword)
                            Spacer()
                            //                            ClearButtonView()
                            HStack{
                                Button(action:{
                                    formViewModel.clear()
                                }){
                                    Text("Clear")
                                        .frame(width: buttonWidth, height: buttonHeight, alignment: .center)
                                        .font(.system(size: 18))
                                    
                                }
                                .foregroundColor(Color.white)
                                .background(Color.blue)
                                .cornerRadius(10)
                                .buttonStyle(BorderlessButtonStyle())
                            }
                            .padding([.leading, .trailing], buttonPaddingX)
                            .padding([.bottom, .top], buttonPaddingY)
                            
                            //                            .frame(width: 160, height: 90, alignment: .center)
                            
                            //                            .buttonStyle(.bordered)
                        }
                    }
                    
                    Section{
                        //                        Text(formViewModel.keyword)
                        Text("Results")
                            .font(.system(size: 27,weight:.bold))
                        
                        if(formViewModel.submitted == true && formViewModel.noresult==false && formViewModel.resultData.count == 0){
                            ProgressView()
                            { Text("Please wait...") }
                            //                                .background(Color.red)
                                .frame(maxWidth: .infinity, alignment: .center)
                        }
                        
                        if(formViewModel.noresult){
                            Text("No result available")
                                .foregroundColor(Color.red)
                        }
                        List(formViewModel.resultData,id:\.self){
                            result in
                            NavigationLink(destination:BusinessDetailsView(businessID: result.id, businessDetailsViewModel:BusinessDetailsViewModel(businessID:result.id, name: result.name) ),label:{
                                HStack{
                                    Text("\(result.index)")
                                        .frame(width: 19, height: resultListHeight,alignment: .leading)
                                    //                                        .padding(.leading,-1)
                                    //                                    AsyncImage(url: URL(string: result.image_url),content: (AsyncImagePhase))
                                    //                                    AsyncImage(url: URL(string: result.image_url)){ image in
                                    AsyncImage(url: URL(string: result.image_url),content: {image in
                                        image
                                            .resizable()
                                            .scaledToFill()
                                            .frame(width: 50, height: resultListHeight)
                                            .cornerRadius(5)
                                        //                                            .aspectRatio(contentMode: .fit)
                                    }, placeholder: {
                                        ProgressView()
                                    })
                                    .frame(width: 50, height: resultListHeight)
                                    //                                    })
                                    
                                    //                                    .frame(width: 50, height: 50)
                                    //                                    .padding()
                                    //                                    .cornerRadius(20)
                                    //                                    .padding()
                                    
                                    Text(result.name)
                                        .frame(width: 130, height: resultListHeight)
                                        .foregroundColor(.gray)
                                    Text(result.rating)
                                        .frame(width: 50, height: resultListHeight,alignment:.leading)
                                        .font(.system(size: 18,weight:.semibold))
                                    Text(result.distance)
                                        .frame(width: 15, height: resultListHeight)
                                        .font(.system(size: 18,weight:.semibold))
                                }}
                            )
                        }
                        
                    }
                }
                .navigationTitle("Business Search")
            }
            .navigationBarItems(
                trailing: NavigationLink(destination: BookingsView()) {
                    Image(systemName: "calendar.badge.clock").imageScale(.large)
                    
                })
            
        }
        .environmentObject(reservationFormViewModel)
        .environmentObject(formViewModel)
        .environmentObject(autoCompleteModel)
        //        .toast(isShowing:showText,text:Text("hello toast"))
        
    }
    
    init() {
        //        formViewModel.distance = State(initialValue: "10")
        //        _keyword = State(initialValue: "food")
        //        _location = State(initialValue: "NYU")
    }
}






struct Business:Hashable{
    var id:String
    var index:Int
    var image_url:String
    var name:String
    var rating:String
    var distance:String
    init(id:String,index: Int, image_url:String, name: String,rating:String,distance:String) {
        self.id = id
        self.index=index
        self.image_url=image_url
        self.name = name
        self.rating=rating
        self.distance=distance
    }
    
}


class FormViewModel:ObservableObject{
    //    private var formModel=FormModel()
    @Published var submitted=false
    @Published var noresult:Bool=false;
    @Published var keyword="";
    @Published var distance="10";
    @Published var auto_detect=false;
    @Published var locaiton="";
    //    @Published var results:NSDictionary=["businesses":[]];
    @Published var selectedCate="All";
    //    @Published var resultData=[Business(id: 0, name: "testname")]
    @Published var resultData:[Business]=[]
    private var lat:String=""
    private var lng:String=""
    @Published var selection:String="Default"
    private var radius:String=""
    private var location:String=""
    private var category:String=""
    var url:String="https://yelp-frontend-8-jfb.wl.r.appspot.com/api/yelpSearch"
    
    var IPInfoUrl="https://ipinfo.io/?token=f20bca2f6b9588"
    let optionTagMap:[String:String]=["Default":"all","Arts and Entertainment":"arts","Health and Medical":"health",
                                      "Hotel and Travel":"hotelstravel","Food":"food","Professional Services":"professional"]
    var geocodeURL="https://yelp-frontend-8-jfb.wl.r.appspot.com/api/geocode?address="
    
    init(){
        //        resultData=[
        //            Business(id:0,name:"testname")
        //        ]
    }
    
    func validateText()->Bool{
        let k=self.keyword
        let d=self.distance
        let l=self.locaiton
        let a=self.auto_detect
        //        print("validate: keyword=\(self.keyword) distance=\(self.distance) location=\(self.locaiton)")
        if(k==""){
            //            print("validate return false1")
            return false
        }
        if(d==""){
            //            print("validate return false2")
            return false
        }
        if(a==false){
            //            print("validate return false3")
            if(l==""){
                //                print("validate return false4")
                //                print("location=\(self.locaiton)")
                return false
            }else{
                //                print("validate return true0")
                return true
            }
        }
        //        print("validate return true")
        return true
    }
    
    func clear(){
        self.resultData=[]
        self.keyword=""
        self.distance="10"
        self.locaiton=""
        self.selection="Default"
        self.noresult=false
        self.auto_detect=false
    }
    
    func submit(){
        self.submitted=true;
        self.noresult=false
        self.resultData=[]
        //        self.keyword=keyword
        //        self.auto_detect=auto_detect
        //        self.locaiton=location
        var d:Double=(Double(self.distance) ?? -1) as Double
        
        //        print("d=\(d)")
        if(d<0){
            self.noresult=true;
            self.submitted=false
            return
        }
        if(d==0){
            d=10
        }
        self.radius=String(Int(d*1609.34))
        //        print("")
        //        print("selection input=\(self.selection)")
        self.category=optionTagMap[self.selection] as! String
        
        //        print("selection=\(self.selection)")
        //        print("location=\(self.locaiton)")
        if(self.auto_detect==true){
            Task{
                await fetchIPAddress()
                //                await fetchSearchResults(url)
            }
        }else{
            //            print("locaiont!!!=\(self.locaiton)")
            let l=self.locaiton
            //            print("before insert geocodeURL=\(self.geocodeURL)")
            let geoURL=self.geocodeURL+l
            //            print("after insert self.geocodeURL=\(geoURL)")
            Task{
                await geocode(geoURL)
            }
        }
    }
    
    func geocode(_ geoURL:String) async{
        if let url=URL(string: (geoURL)){
            //            print("inside geocode requesting\(url)")
            try await AF.request(url).validate().responseJSON{
                (response) in
                switch response.result{
                case .success(let JSON):
                    //                    print("geocode address succeed")
                    let response=JSON as! NSDictionary
                    //                    print("geocode response=\(response)")
                    //                    let re=response.object(forKey: "results") as! NSDictionary
                    if(response.object(forKey: "results") != nil){
                        let re=response.object(forKey: "results") as! NSArray
                        if(re.count != 0){
                            let re=re[0] as! NSDictionary
                            if(re.object(forKey: "geometry") != nil){
                                let geo=re.object(forKey: "geometry") as! NSDictionary
                                if(geo.object(forKey: "location") != nil){
                                    let lo=geo.object(forKey: "location") as! NSDictionary
                                    if(lo.object(forKey: "lat") != nil && lo.object(forKey: "lng") != nil){
                                        //                                        print("contain key lat and lng")
                                        let la=lo.object(forKey: "lat") as! Double
                                        let ln=lo.object(forKey: "lng") as! Double
                                        self.lat="\(la)"
                                        self.lng="\(ln)"
                                    }else{
                                        self.noresult=true;
                                        self.submitted=false
                                        return
                                    }
                                    
                                }else{
                                    self.noresult=true;
                                    self.submitted=false
                                    return
                                }
                            }else{
                                self.noresult=true;
                                self.submitted=false
                                return
                            }
                        }else{
                            self.noresult=true;
                            self.submitted=false
                            return
                        }
                    }else{
                        //                        print("doesn't contain key lat lng")
                        self.noresult=true;
                        self.submitted=false
                        return;
                    }
                    let newURL="\(self.url)?term=\(self.keyword)&latitude=\(self.lat)&longitude=\(self.lng)&categories=\(self.selection)&radius=\(self.radius)"
                    self.fetchResults(newURL: newURL)
                case .failure(let error):
                    print("geocode address fail")
                }
                //                print("inside try await")
            }
            //            print("ouside try await")
        }
    }
    
    func fetchResults(newURL:String){
        
        Task{
            await fetchSearchResults(newURL)
        }
    }
    
    func fetchSearchResults(_ url:String)async{
        if let url=URL(string:(url)){
            try await AF.request(url).validate().responseJSON{
                (response) in
                switch response.result{
                case .success(let JSON):
                    let response=JSON as! NSDictionary
                    let businesses=response.object(forKey:"businesses")! as! [Any]
                    var num=1
                    if(businesses.count == 0){
                        self.noresult=true
                    }
                    businesses.forEach{
                        b in
                        var dis=(b as AnyObject).object(forKey:"distance") as! Double
                        dis=dis/1609.34
                        let disStr=String(format: "%.0f", dis)
                        let rat=(b as AnyObject).object(forKey:"rating") as! Double
                        let ratStr=String(format:"%.1f",rat)
                        self.resultData.append(Business(id:(b as AnyObject).object(forKey:"id") as! String,index:num,image_url:(b as AnyObject).object(forKey:"image_url") as! String,
                                                        name:(b as AnyObject).object(forKey:"name") as! String,
                                                        rating:ratStr, distance: disStr))
                        num=num+1
                    }
                    self.submitted=false
                    
                case .failure(_):
                    self.noresult=true;
                    self.submitted=false
                }
            }
        }
    }
    
    
    func fetchIPAddress()async{
        if let url=URL(string: (IPInfoUrl)){
            //            print("inside fetch ipinfo requesting:\(url)")
            try await AF.request(url).validate().responseJSON{
                (response) in
                switch response.result{
                case .success(let JSON):
                    //                    print("fetch ip info success")
                    let response=JSON as! NSDictionary
                    //                    print("ip info result=\(response)")
                    if(response.object(forKey: "loc") != nil){
                        //                        print("contain key loc")
                        let latlng=response.object(forKey: "loc") as! String
                        let latlngs=latlng.split(separator: ",")
                        self.lat=String(latlngs[0])
                        self.lng=String(latlngs[1])
                        //                        print("self.lat=\(self.lat)")
                        //                        print("self.lng=\(self.lng)")
                        //                        print("selection inside ipinfo fetch=\(self.selection)")
                        let newURL="\(self.url)?term=\(self.keyword)&latitude=\(self.lat)&longitude=\(self.lng)&categories=all&radius=\(self.radius)"
                        
                        self.fetchResults(newURL: newURL)
                    }else{
                        //                        print("contain no key loc")
                        self.noresult=true
                        self.submitted=false
                    }
                case .failure(let error):
                    self.noresult=true
                    self.submitted=false
                    //                    print("fetch ip info fail")
                }
            }
        }
    }
    
}





//struct SampleDetails: View {
//    @Environment(\.presentationMode) var presentationMode: Binding<PresentationMode>
//
//    var btnBack : some View { Button(action: {
//        self.presentationMode.wrappedValue.dismiss()
//    }) {
//        HStack {
//            Image("ic_back") // set image here
//                .aspectRatio(contentMode: .fit)
//                .foregroundColor(.white)
//            Text("< Back")
//        }
//    }
//    }
//
//    var body: some View {
//        List {
//            Text("sample code")
//        }
//        .navigationBarBackButtonHidden(true)
//        .navigationBarItems(leading: btnBack)
//    }
//}



struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
        //            .background(Color.gray)
    }
}




//class ReservationFormViewModel:ObservableObject{
//    //    @Published var name:String=""
//    //    @Published var businessID:String=""
//    @Published var email:String=""
//    @Published var datee:Date=Date()
//    @Published var date:String=""
//    @Published var hour:String=""
//    @Published var minute:String=""
//    @Published var reservations:[Reservation]=[]
//    @Published var reservationIndexMap:[String:Int]=[:]
//    @Published var dismissed:Bool=false
//    //    @EnvironmentObject var businessDetailsViewModel:BusinessDetailsViewModel
//    private var index=1;
//    init(){
//        self.index=1
//        //        print("reservation form init")
//    }
//    func addReservation(name:String,businessID:String){
//        if self.checkForKey(businessID: businessID){
//            return
//        }
//        //        self.addInRImap(businessID:businessID);
//        //        let dateformatter=DateFormatter()
//        //        let da=dateformatter.string(from:self.datee)
//        let da="\(self.datee)"
//        //        print("datee=\(self.datee)")
//        //        print("da=\(da)")
//        let datetime=da.components(separatedBy: " ")
//        //        print("datetime=\(datetime)")
//                print("before adding:\(self.reservations)")
//        let time="\(self.hour):\(self.minute)"
//        self.reservations.append(Reservation(name:name,businessID:businessID, email: email, date: datetime[0], time: time))
//        self.addInRImap(businessID:businessID);
//                print("after adding:\(self.reservations)")
//    }
//
//    func cancelReservation(businessID:String){
//                print("before remove:\(self.reservations)")
//        //        self.reservations.removeValue(forKey: businessID)
//        let pos=self.reservationIndexMap[businessID] as! Int
//        self.removeInRImap(businessID: businessID)
//        self.reservations.remove(at:pos-1)
//
//                print("after remove:\(self.reservations)")
//    }
//
//    func addInRImap(businessID:String){
//                print("before index:\(self.reservationIndexMap)")
//        self.reservationIndexMap[businessID]=index;
//        index=index+1;
//                print("after index:\(self.reservationIndexMap)")
//    }
//
//    func removeInRImap(businessID:String){
//                print("before remove index:\(self.reservationIndexMap)")
//        let pos=self.reservationIndexMap[businessID] as! Int
//        let len=self.reservations.count as! Int
//        //        print("pos=\(pos)")
//        //        print("len=\(len)")
//        if(pos<=len-1){
//            for i in pos...len-1{
//                let bID=self.reservations[i].businessID as! String
//                self.reservationIndexMap[bID]=i
//                //                print("i=\(i) bID=\(bID)")
//                //                print("after this iteration:\(self.reservationIndexMap)")
//            }
//        }
//        self.reservationIndexMap[businessID]=nil
//        //        ForEach(Array(self.reservationIndexMap.keys),id:\.self){
//        //
//        //        }
//        self.index=self.index-1
//                print("after remove index:\(self.reservationIndexMap)")
//    }
//
//    func checkForKey(businessID:String) -> Bool{
//        if(self.reservationIndexMap[businessID] != nil){
//            return true;
//        }else{
//            return false
//        }
//    }
//
//    func checkForBusinessID(index:Int)->String{
//        return self.reservations[index].businessID
//    }
//}
//
