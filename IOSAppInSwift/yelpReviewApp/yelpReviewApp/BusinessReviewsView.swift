//
//  BusinessReviewsView.swift
//  yelp-9
//
//  Created by Xintong Chen on 11/25/22.
//

import SwiftUI
import Alamofire
import SwiftyJSON
import MapKit

struct BusinessReview:Hashable{
    let name:String
    let rating:String
    let text:String
    let time:String
    init(name:String,rating:String,text:String,time:String){
        self.name=name;
        self.rating=rating
        self.text=text
        self.time=time
    }
}

class BusinessReviewsViewModel:ObservableObject{
    var businessID:String;
    private var url:String="https://yelp-frontend-8-jfb.wl.r.appspot.com/api/reviews"
    @Published var businessReviews:[BusinessReview]=[]
    
    init(businessID:String){
        self.businessID=businessID
        self.url=self.url+"?id=\(self.businessID)"
    }
    func fetchData(){
//        print("fetch reviews")
        Task{
            await fetchReviews(self.url)
        }
    }
    func fetchReviews(_ url:String)async{
        if let url=URL(string: (url)){
            try await AF.request(url).validate().responseJSON{
                (response) in
                switch response.result{
                case .success(let JSON):
                    self.businessReviews=[]
                    let response=JSON as! NSDictionary
                    let reviews=response.object(forKey: "reviews") as! NSArray
//                    print("fetch reviews success")
//                    print("response=\(response)")
                    reviews.forEach{
                        review in
                        //                        let re=review
                        let user=(review as AnyObject).object(forKey:"user") as! NSDictionary
                        let username=user.object(forKey: "name") as! String
                        let rating=(review as AnyObject).object(forKey: "rating") as! Double
                        var ratStr=String(format: "%.0f", rating)
                        ratStr="\(ratStr)/5"
                        let text=(review as AnyObject).object(forKey: "text") as! String
                        var time=(review as AnyObject).object(forKey: "time_created") as! String
                        time=time.components(separatedBy: " ")[0]
                        self.businessReviews.append(BusinessReview(name: username, rating: ratStr, text: text, time: time))
                    }
//                    print("reviews=\(self.businessReviews)")
                case .failure(let error):
                    print("fetch reviews fail")
                }
            }
        }
    }
}




struct BusinessReviewsView:View{
    @ObservedObject var businessReviewsViewModel:BusinessReviewsViewModel
    
    var body: some View{
//        if(businessReviewsViewModel.businessReviews.count == 0){
//            ProgressView()
//                .onAppear{
//                    businessReviewsViewModel.fetchData()
//                }
//        }else{
        List(businessReviewsViewModel.businessReviews,id:\.self){
            review in
            VStack{
                HStack{
                    VStack(alignment: .leading){
                        HStack{
                            Spacer()
                        }
                        HStack{
                            Text(review.name)
                                .font(.system(size: 18,weight:.bold))
                        }
                    }
                    VStack(alignment: .trailing){
                        HStack{
                            Spacer()
                        }
                        HStack{
                            Text("\(review.rating)")
                                .frame(width: .infinity)
                                .font(.system(size: 18,weight:.bold))
                        }
                    }
                }
                Text(review.text)
                    .font(.system(size: 18))
                    .foregroundColor(.gray)
                    .padding(.top,2)
                Text(review.time)
                    .padding(.top,2)
                    .padding(.bottom,5)
                    .font(.system(size: 18,weight:.bold))
            }
            .padding([.leading,.trailing],15)
            .padding(.top,5)
        }
        .padding(.top,0)
        .onAppear{
            businessReviewsViewModel.fetchData()
        }
//        }
           
    }
    
    
}

//struct BusinessReviewsView_Previews: PreviewProvider {
//    static var previews: some View {
//        BusinessReviewsView()
//    }
//}
