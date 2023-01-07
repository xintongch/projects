import { Injectable } from '@angular/core';
import { apidataService } from './api-data.service';
import { DisplayDetailsService } from './display-details.service';

@Injectable({
  providedIn: 'root',
})
export class DisplayResultsService {
  constructor(
    private apidataService: apidataService,
    private displayDetailsService: DisplayDetailsService
  ) {}

  clickBusinessName(obj: any) {
    this.apidataService.yelpDetail(obj).subscribe((detail) => {
      this.displayDetailsService.displayDetails(detail);
    });
  }

  displayResults(results: any) {
    if(!results.hasOwnProperty('businesses')){
      $('#no-result').css({
        display: 'inline-block',
      });
      $('#rt-container').css({
        display: 'none',
      });
      return;
    }
    if (results.businesses.length == 0) {
      $('#no-result').css({
        display: 'inline-block',
      });
      $('#rt-container').css({
        display: 'none',
      });
      return;
    }
    $('#no-result').css({
      display: 'none',
    });
    let data: any = [];
    let result_table_container = $('#rt-container');
    let result_table_body = $('#rt-container table tbody');
    result_table_body.children().remove();

    let index = 0;
    results.businesses.forEach((element: any) => {
      index += 1;
      if (index > 10) {
        return;
      }
      result_table_body.append(
        $(
          "<tr class='hover-pointer click-rt align-top border-bottom rt-row' id=" +
            element.id +
            " ><th class='text-left pt-2 lp' scope='row'>" +
            index +
            "</th><td class='align-middle'><img src='" +
            element.image_url +
            "' class='img-fluid result-table-img'></td><td class='pt-lg-3 pt-2'><div class=''>" +
            element.name +
            "</div></td><td class='pt-lg-3 pt-2'>" +
            element.rating +
            "</td><td class='pt-2 pt-lg-3'>" +
            (element.distance / 1609.34).toFixed(2) +
            '</td></tr>'
        )
      );
    });

    $('.hover-pointer').css({
      cursor: 'pointer',
    });

    $('#rt-container table tbody tr:nth-child(odd)').css({
      'background-color': 'rgb(240, 240, 240)',
    });
    let self = this;
    $('.click-rt').click(function () {
      self.clickBusinessName(this.id);
    });

    $('#rt-container').css({
      'display':'inline-block'
    })
    



    let largeScreen = window.matchMedia('(min-width: 576px)');
    if (largeScreen.matches) {
    $('.result-table-img').css({
      height: '98px',
      width: '98px',
      'object-fit': 'fill',
    });

    $('.rt-row').css({
      'height':'116px',
      'font-size':'16px'
    })
    $('.lp').css({
      'padding-left':'24px'
    })
    
  }

    let smallScreen = window.matchMedia('(max-width: 576px)');
    if (smallScreen.matches) {
      
      $('.result-table-img').css({
        height: '50px',
        width: '50px',
        'object-fit': 'fill',
      });
      $('.rt-row').css({
        'height':'70px',
        'font-size':'13px'
      })

      $('.lp').css({
        'padding-left':'11.5px'
      })
    }

  }
}
