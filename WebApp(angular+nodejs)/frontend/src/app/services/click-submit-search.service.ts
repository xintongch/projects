import { Injectable } from '@angular/core';
import { apidataService } from './api-data.service';
import { DisplayResultsService } from './display-results.service';

@Injectable({
  providedIn: 'root',
})
export class ClickSubmitSearchService {
  constructor(
    private apidataService: apidataService,
    private displayResultsService: DisplayResultsService
  ) {}

  submitSearch(lat: any, lng: any): void {
    let term = (<HTMLInputElement>document.getElementById('keyword')).value;
    let radius: number = Math.round(
      Number((<HTMLInputElement>document.getElementById('distance')).value) *
      1609.34
      );
      if(isNaN(radius)){
        $('#no-result').css({
          display: 'inline-block',
        });
        $("#rt-container").css({
          'display':'none'
        })
        return;
      }
    if(radius==0){
      radius= Math.round(16093.4);
    }
    if(radius>40000){
      $('#no-result').css({
        display: 'inline-block',
      });
      $('#rt-container').css({
        display: 'none',
      });
      return;
    }
    let categories = (<HTMLInputElement>document.getElementById('category'))
      .value;
    let location = (<HTMLInputElement>document.getElementById('location'))
      .value;
    this.apidataService
      .yelpSearch(term, lat, lng, categories, radius)
      .subscribe((res) => {
        console.log('ress=',res)
        this.displayResultsService.displayResults(res);
      });
  }

  clickSubmit() {
    $('#dc-container').css({ display: 'none' });
    if ($('#keyword').val() == '') {
      (<HTMLInputElement>(
        document.getElementById('search-form')
      )).reportValidity();
      return;
    }
    if ((<HTMLInputElement>document.getElementById('auto-detect')).checked) {
      this.apidataService.ipInfo().subscribe((ipInfo: any) => {
        if (!ipInfo.hasOwnProperty('loc')) {
          $('#no-result').css({
            display: 'inline-block',
          });
          $("#rt-container").css({
            'display':'none'
          })
          return;
        }else{
          let latlng = ipInfo.loc.split(',');
          let lat = latlng[0];
          let lng = latlng[1];
          this.submitSearch(lat, lng);
        }
      });
    } else {
      if ($('#location').val() == '') {
        (<HTMLInputElement>(
          document.getElementById('search-form')
        )).reportValidity();
        return;
      }
      let location = (<HTMLInputElement>document.getElementById('location'))
        .value;
      this.apidataService.geocode(location).subscribe((geo) => {
        if (geo.results.length > 0 && geo.results[0].hasOwnProperty("geometry") && geo.results[0].geometry.hasOwnProperty("location") && geo.results[0].geometry.location.hasOwnProperty("lat")) {
          let lat = geo.results[0].geometry.location.lat;
          let lng = geo.results[0].geometry.location.lng;
          this.submitSearch(lat, lng);
        }else{
          $('#no-result').css({
            display: 'inline-block',
          });
          $("#rt-container").css({
            'display':'none'
          })
        }
      });
    }
  }
}
