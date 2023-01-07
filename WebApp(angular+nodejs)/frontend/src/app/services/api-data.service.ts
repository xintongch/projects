import { Injectable } from '@angular/core';
import { HttpClient, HttpHeaders, HttpParams} from '@angular/common/http'; 


@Injectable({
  providedIn: 'root'
})
export class apidataService {
  private headers=new HttpHeaders().set('Authorization','Bearer '+'rYMJu1j6WPJ7XVFobeWpUqh9eHVHBmdPF-VbMr1uR4k1Aakh12gzs0evUrB2MEyZce_WOIqv_8TjgB-aYNEgbEpsVBiTaDppLLim6kbaQzNBNZkz2F995PaWGeAcY3Yx')
  constructor(private http:HttpClient) { }
  
  geocode(location:any){
    let geocodingUrl = "/api/geocode?address="+location;
    // let params=new HttpParams().set('address',location);
    let headers=this.headers;
    return this.http.get<any>(geocodingUrl)
    // return this.http.get<any>(geocodingUrl,{params})
  }
  // geocode(location:any){
  //   let geocodingUrl = "https://maps.googleapis.com/maps/api/geocode/json?address="+location+"&key=AIzaSyDX-JdhG_Ft90f6gtYGvBtNTqtdr9pZNE4";
  //   console.log('serviceeeeeee')
  //   console.log(geocodingUrl);
  //   return this.http.get<any>(geocodingUrl)
  // }
  
  yelpSearch(term:any,lat:number,lng:number,categories:any,radius:any){
    let yelpURL = "/api/yelpSearch";
    let params=new HttpParams().set('term',term).set('latitude',lat).set('longitude',lng).set('categories',categories).set('radius',radius);
    let headers=this.headers;
    return this.http.get<any>(yelpURL,{params,headers})
  }


  yelpDetail(obj:any){
    let yelpDetailsURL = '/api/detail?id='+obj;
    return this.http.get<any>(yelpDetailsURL);
  }

  yelpReviews(id:any){
    let yelpReviewsURL="/api/reviews?id="+id;
    return this.http.get<any>(yelpReviewsURL);
  }


  autoComplete(value:any){
    if(value==''){
      return '';
    }
    let autoCompleteURL="/api/autoComplete?text="+value;
    return this.http.get(autoCompleteURL);
  }

  ipInfo(){
    let ipInfoURL="https://ipinfo.io/?token=f20bca2f6b9588";
    return this.http.get(ipInfoURL)
  }

}
