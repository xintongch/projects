//
//  AutoComplete.swift
//  yelp-9
//
//  Created by Xintong Chen on 11/27/22.
//

import SwiftUI
import Alamofire
//import SwiftyJSON
import MapKit


class AutoCompleteModel:ObservableObject{
    private var keyword:String=""
    @Published var fetching:Bool=false
    private var url:String="https://yelp-frontend-8-jfb.wl.r.appspot.com/api/autoComplete"
    @Published var autoCompleteList:[String]=[]
    init(){
        
    }
    func getAutoComplete(_ keyword:String){
        self.fetching=true
        self.autoCompleteList=[]
        self.keyword=keyword
        let ACURL="\(self.url)?text=\(keyword)"
        Task{
            await fetchAutoComplete(ACURL: ACURL)
        }
    }
    func fetchAutoComplete(ACURL:String)async{
        if let url=URL(string: (ACURL)){
//            print("requesting:\(url)")
            try await AF.request(url).validate().responseJSON{
                (response) in
                switch response.result{
                case .success(let JSON):
                    let response=JSON as! NSDictionary
//                    print("fetch autocomplete success")
//                    print("autocomplete response=\(response)")
                    if(response.object(forKey: "categories") != nil){
                        let ca=response.object(forKey: "categories") as! NSArray
                        ca.forEach{
                            c in
                            if((c as AnyObject).object(forKey: "title") != nil){
                                self.autoCompleteList.append((c as AnyObject).object(forKey: "title") as! String)
                            }
                        }
                    }else{
//                        print("contain no key categories")
                    }
                    
                    if(response.object(forKey: "terms") != nil){
                        let te=response.object(forKey: "terms") as! NSArray
                        te.forEach{
                            t in
                            if((t as AnyObject).object(forKey: "text") != nil){
                                self.autoCompleteList.append((t as AnyObject).object(forKey: "text") as! String)
                            }
                        }
                    }else{
//                        print("contain no key terms")
                    }
//                    print("autocompletelist=\(self.autoCompleteList)")
                    self.fetching=false
                case .failure(let error):
                    self.fetching=false;
//                    print("fetch autocomplete fail")
                }
            }
        }
    }
}



//struct AutoCompleteView:View{
//    @EnvironmentObject var autoCompleteModel:AutoCompleteModel
//    var body: some View{
//
//    }
//}


//struct AutoComplete: View {
//    var body: some View {
//        Text(/*@START_MENU_TOKEN@*/"Hello, World!"/*@END_MENU_TOKEN@*/)
//    }
//}

//struct AutoComplete_Previews: PreviewProvider {
//    static var previews: some View {
//        AutoComplete()
//    }
//}
