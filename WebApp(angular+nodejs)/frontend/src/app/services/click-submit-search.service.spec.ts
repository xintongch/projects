import { TestBed } from '@angular/core/testing';

import { ClickSubmitSearchService } from './click-submit-search.service';

describe('ClickSubmitSearchService', () => {
  let service: ClickSubmitSearchService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(ClickSubmitSearchService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
